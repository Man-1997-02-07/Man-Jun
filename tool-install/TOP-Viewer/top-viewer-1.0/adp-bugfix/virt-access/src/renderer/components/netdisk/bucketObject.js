export default {
  data () {
    return {
      isNotClick: false,
      loadingHidden: ''
    }
  },
  mixins: [],
  methods: {
    // 获取文件夹下的文件列表
    getBucketObject (bucketsPath, isBack) {
      const self = this
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.cur_fileLength = 0
      self.uploadingTimer && clearInterval(self.uploadingTimer)
      self.uploadingTimer = null
      self.isNewFiles = false
      self.isRename = false
      self.isAdd = true
      self.fileNameTips = ''
      self.fileName = this.$t('disk.addFolder')
      if (!self.uploadLoading) {
        self.setState({
          attr: ((self.dialogMoveVisible || self.dialogCopyVisible) && isBack) ? 'innerLoading' : 'onLoading',
          val: true
        })
      }
      let params = {}
      self.curFolders = []
      // self.upload_fileList = [];
      if ((self.dialogMoveVisible || self.dialogCopyVisible) && isBack) {
        self.allBuckets = []
        params = {
          Bucket: bucketsPath[0]
        }
        if (bucketsPath && bucketsPath.length > 1) {
          params.Prefix = bucketsPath.slice(1, (bucketsPath.length)).join('/') + '/'
        }
      } else if (bucketsPath && isBack) {
        params = {
          Bucket: bucketsPath[0],
          Prefix: bucketsPath.slice(1, (bucketsPath.length)).join('/') + '/',
          Marker: (self.pagenum - 1).toString(),
          MaxKeys: self.pagesize
        }
      } else {
        params = {
          Bucket: self.cur_buckets[0],
          Marker: (self.pagenum - 1).toString(),
          MaxKeys: self.pagesize
        }
        if (self.cur_buckets.length > 1) {
          params.Prefix = self.cur_buckets.slice(1, (self.cur_buckets.length)).join('/') + '/'
        }
      }
      self.s3UploadRequest = self.awsS3Client.listObjects(params, function (err, data) {
        if (!self.uploadLoading) {
          self.setState({
            attr: ((self.dialogMoveVisible || self.dialogCopyVisible) && isBack) ? 'innerLoading' : 'onLoading',
            val: false
          })
        }
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 1500
          })
        } else {
          if ((self.dialogMoveVisible || self.dialogCopyVisible) && isBack) {
            if (self.moveFileObj || self.copyFileObj) {
              for (let i = 0; i < data.CommonPrefixes.length; i++) {
                const obj = data.CommonPrefixes[i]
                const num = bucketsPath.length > 1 ? obj.Prefix.split('/')[bucketsPath.length - 1] : obj.Prefix.split('/')[0]
                if ((bucketsPath.join('/') !== self.cur_buckets.join('/')) || (self[self.moveFileObj ? 'moveFileObj' : 'copyFileObj'].name !== num)) {
                  self.allBuckets.push({
                    bucket: obj.Prefix.split('/')[bucketsPath.length - 2],
                    size: '-',
                    name: bucketsPath.length > 1 ? obj.Prefix.split('/')[bucketsPath.length - 1] : obj.Prefix.split('/')[0],
                    type: 'folder',
                    isSelected: true
                  })
                }
              }
            } else {
              for (let i = 0; i < data.CommonPrefixes.length; i++) {
                const obj = data.CommonPrefixes[i]
                const num = bucketsPath.length > 1 ? obj.Prefix.split('/')[bucketsPath.length - 1] : obj.Prefix.split('/')[0]
                var isExist = false
                for (var j = 0; j < self.networkDataSelect.length; j++) {
                  var aj = self.networkDataSelect[j]
                  var n = aj.name
                  if (n === num && bucketsPath.join('/') === self.cur_buckets.join('/')) {
                    isExist = true
                    break
                  }
                }
                if (!isExist) {
                  self.allBuckets.push({
                    bucket: obj.Prefix.split('/')[bucketsPath.length - 2],
                    size: '-',
                    name: bucketsPath.length > 1 ? obj.Prefix.split('/')[bucketsPath.length - 1] : obj.Prefix.split('/')[0],
                    type: 'folder',
                    isSelected: true
                  })
                }
              }
            }
            self.moveDiaFiles = []
            self.copyDiaFiles = []
            if (self.dialogMoveVisible) {
              self.moveDiaFiles = data.Contents.concat(data.CommonPrefixes)
            }
            if (self.dialogCopyVisible) {
              self.copyDiaFiles = data.Contents.concat(data.CommonPrefixes)
            }
          } else {
            self.files = []
            self.notMatchCurFolderAccount = 0
            data.Contents.forEach((item) => {
              // if (item.ETag !== '"00000000000000000000000000000000-1"') {
              self.uploadLoading = false
              self.files.push({
                bucket: data.Prefix.split('/')[self.cur_buckets.length - 1],
                name: item.Key,
                tag: item.ETag,
                access: self.cur_file_auth,
                size: self.parternFileSize(item.Size),
                byteNum: parseInt(item.Size),
                id: item.Owner.ID,
                type: item.Key.split('.')[item.Key.split('.').length - 1] || '',
                // type: item.StorageClass,
                mtime: self.parternTime(item.LastModified)
              })
              self.uploadingTimer && clearInterval(self.uploadingTimer)
              self.uploadingTimer = null
              // } else {
              // self.notMatchCurFolderAccount++
              // if (!self.cur_progress) {
              //   console.log(123)
              //   self.uploadLoading = true
              //   self.uploadingTimer = setInterval(function () {
              //     self.getBucketObject()
              //   }, 1000)
              // }
              // }
            })
            data.CommonPrefixes.forEach(item => {
              self.curFolders.forEach(v => {
                if (decodeURIComponent(v.label) === item.Prefix.toUpperCase()) {
                  self.uploadLoading = false
                  self.files.push({
                    bucket: item.Prefix.split('/')[self.cur_buckets.length - 2],
                    size: v.size,
                    byteNum: v.byteNum,
                    mtime: v.mtime,
                    access: self.cur_file_auth,
                    name: self.cur_buckets.length > 1 ? item.Prefix.split('/')[self.cur_buckets.length - 1] : (item.Prefix.split('/')[0] !== '' ? item.Prefix.split('/')[0] : item.Prefix.split('/')[1]),
                    type: 'folder'
                  })
                }
              })
            })
            self.cur_fileLength = self.files.length
            self.files.forEach((item, idx) => {
              item.index = idx + 1 + self.pagenum * self.pagesize
            })
            self.isShowUpload = false
          }
        }
      })
      self.s3UploadRequest.on('httpHeaders', function (statusCode, headers) {
        self.total = parseInt(headers['x-amz-object-count'])
        const allFolders = []
        for (const key in headers) {
          if (key.indexOf('x-amz-dir-') > -1) {
            allFolders.push(headers[key])
          }
        }
        allFolders.forEach(item => {
          self.curFolders.push({
            label: (item.split('#')[0]).toUpperCase(),
            size: self.parternFileSize(item.split('#')[1]),
            byteNum: parseInt(item.split('#')[1]),
            mtime: new Date(+new Date(item.split('#')[2]) + 8 * 3600 * 1000).toISOString().replace(/T/g, ' ').replace(/\.[\d]{3}Z/, '')
          })
        })
      })
      this.loadingHidden.close()
    },
    // 返回上一级
    backBeforeBucket (bucketsPath) {
      const self = this
      self.isNewFiles = false
      self.isRename = false
      self.isAdd = true
      self.fileNameTips = ''
      self.filtersVal = ''
      self.fileName = this.$t('disk.addFolder')
      self.$Api.system.updateWsToken()
      self.setState({
        attr: (self.dialogMoveVisible || self.dialogCopyVisible) ? 'innerLoading' : 'onLoading',
        val: true
      })
      self.cur_fileLength = 0
      self.pagenum = 1
      if (self.dialogMoveVisible || self.dialogCopyVisible) {
        self.allBuckets = []
        if (bucketsPath && bucketsPath.length > 1) {
          bucketsPath.splice((bucketsPath.length - 1), 1)
          if (self.cur_buckets.join('/') === bucketsPath.join('/')) {
            self.isNotClick = true
          } else {
            if (self.cur_buckets.join('/') === bucketsPath.slice(0, self.cur_buckets.length).join('/')) {
              if (!self.cur_buckets.length) {
                self.isNotClick = false
              }
            } else {
              self.isNotClick = false
            }
          }
          self.getBucketObject(bucketsPath, true)
        } else {
          bucketsPath.splice(0, 1)
          backAllBuckets()
        }
      } else {
        self.files = []
        self.curFolders = []
        if (self.cur_buckets.length > 1) {
          self.cur_buckets.splice((self.cur_buckets.length - 1), 1)
          self.getBucketObject()
        } else {
          self.isShow = false
          self.cur_buckets = []
          backAllBuckets()
        }
      }
      function backAllBuckets () {
        self.s3UploadRequest = self.awsS3Client.listBuckets(function (err, data) {
          self.setState({
            attr: (self.dialogMoveVisible || self.dialogCopyVisible) ? 'innerLoading' : 'onLoading',
            val: false
          })
          if (err) {
            self.$message({
              message: err,
              type: 'error',
              duration: 1500
            })
          } else {
            if (self.dialogMoveVisible || self.dialogCopyVisible) {
              data.Buckets.forEach((item) => {
                if ((self.copyFileObj && item.Name !== self.copyFileObj.name) || (self.moveFileObj && item.Name !== self.moveFileObj.name)) {
                  self.allBuckets.push({
                    name: item.Name,
                    type: 'folder',
                    size: '-',
                    id: data.Owner.ID,
                    mtime: self.parternTime(item.CreationDate),
                    isSelected: true
                  })
                } else if (self.cur_buckets.length > 0) {
                  self.allBuckets.push({
                    name: item.Name,
                    type: 'folder',
                    size: '-',
                    id: data.Owner.ID,
                    mtime: self.parternTime(item.CreationDate),
                    isSelected: true
                  })
                }
              })
            } else {
              if (self.isShow) {
                data.Buckets.forEach((item) => {
                  self.files.push({
                    name: item.Name,
                    type: 'folder',
                    size: '-',
                    id: data.Owner.ID,
                    mtime: self.parternTime(item.CreationDate)
                  })
                })
              } else {
                data.Buckets.forEach((item) => {
                  self.curFolders.forEach(v => {
                    if (decodeURIComponent(v.label) === item.Name.toUpperCase()) {
                      self.files.push({
                        name: item.Name,
                        size: v.size,
                        byteNum: v.byteNum,
                        type: 'folder',
                        mtime: self.parternTime(item.CreationDate),
                        StorageQuota: 0
                      })
                    }
                  })
                })
                if (self.isFromVolume) {
                  self.files.forEach(item => {
                    self.awsS3Client.getBucketQuota({ Bucket: item.name }, function (err, data) {
                      if (err) {
                        self.$message({
                          message: err,
                          type: 'error',
                          duration: 1500
                        })
                      } else {
                        item.StorageQuota = data.StorageQuota
                      }
                    })
                  })
                }
              }
              self.cur_fileLength = data.Buckets.length
            }
          }
        })
        self.s3UploadRequest.on('httpHeaders', function (statusCode, headers) {
          for (const key in headers) {
            if (key.split('x-amz-')[1]) {
              self.curFolders.push({
                label: (key.split('x-amz-')[1]).toUpperCase(),
                size: self.parternFileSize(headers[key]),
                byteNum: parseInt(headers[key])
              })
            }
          }
        })
      }
    },
    // 全部文件
    getAllBuckets () {
      const self = this
      self.$Api.system.updateWsToken()
      self.pagenum = 1
      self.filtersVal = ''
      self.isNewFiles = false
      self.isRename = false
      self.cur_file_auth = ''
      self.isAdd = true
      self.fileNameTips = ''
      self.fileName = this.$t('disk.addFolder')
      self.setState({
        attr: (self.dialogMoveVisible || self.dialogCopyVisible) ? 'innerLoading' : 'onLoading',
        val: true
      })
      // eslint-disable-next-line standard/object-curly-even-spacing
      self.loadingHidden = self.$loading({ target: '.netDisk-page'})
      if (self.dialogMoveVisible || self.dialogCopyVisible) {
        self.allBuckets = []
      } else {
        self.cur_fileLength = 0
        self.files = []
        self.cur_buckets = []
        self.curFolders = []
        self.isShow = false
      }
      if (!self.awsS3Client) {
        self.loadingHidden.close()
        return
      }
      self.s3UploadRequest = self.awsS3Client.listBuckets(function (err, data) {
        self.loadingHidden.close()
        self.setState({
          attr: (self.dialogMoveVisible || self.dialogCopyVisible) ? 'innerLoading' : 'onLoading',
          val: false
        })
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 1500
          })
        } else {
          if (self.dialogMoveVisible || self.dialogCopyVisible) {
            data.Buckets.forEach((item) => {
              if ((self.cur_buckets.length === 0 && self[self.dialogMoveVisible ? 'moveFileObj' : 'copyFileObj'].name !== item.Name) || self.cur_buckets.length > 0) {
                self.allBuckets.push({
                  name: item.Name,
                  access: item.Access,
                  type: 'folder',
                  size: '-',
                  id: data.Owner.ID,
                  mtime: self.parternTime(item.CreationDate),
                  isSelected: true
                })
              }
            })
          } else {
            data.Buckets.forEach((item) => {
              self.curFolders.forEach(v => {
                if (decodeURIComponent(v.label) === item.Name.toUpperCase()) {
                  self.files.push({
                    name: item.Name,
                    access: item.Access, // 当前用户的文件权限，READ表示只读，通过这个字段限定操作
                    size: v.size,
                    byteNum: v.byteNum,
                    type: 'folder',
                    mtime: self.parternTime(item.CreationDate),
                    StorageQuota: 0
                  })
                }
              })
            })
            self.cur_fileLength = data.Buckets.length
            if (self.isFromVolume) {
              self.files.forEach(item => {
                self.awsS3Client.getBucketQuota({ Bucket: item.name }, function (err, data) {
                  if (err) {
                    self.$message({
                      message: err,
                      type: 'error',
                      duration: 1500
                    })
                  } else {
                    item.StorageQuota = data.StorageQuota
                  }
                })
              })
            }
          }
        }
      })
      self.s3UploadRequest.on('httpHeaders', function (statusCode, headers) {
        for (const key in headers) {
          if (key.split('x-amz-')[1]) {
            self.curFolders.push({
              label: (key.split('x-amz-')[1]).toUpperCase(),
              size: self.parternFileSize(headers[key]),
              byteNum: parseInt(headers[key])
            })
          }
        }
      })
      self.loadingHidden.close()
    },
    // 刷新页面
    handleNetworkBoxRefresh () {
      if (!this.awsS3Client) {
        this.$message({
          message: '网盘状态不健康，暂时无法获取数据',
          type: 'warning',
          duration: 1500
        })
        return false
      }
      if (this.isShow) {
        this.getBucketObject()
      } else {
        this.getAllBuckets()
      }
      this.isNewFiles = false
      this.isRename = false
      this.isAdd = true
      this.fileName = this.$t('disk.addFolder')
      this.fileNameTips = ''
    },
    bucketPathChange (val) {
      this.isNotClick = val
    }
  }
}

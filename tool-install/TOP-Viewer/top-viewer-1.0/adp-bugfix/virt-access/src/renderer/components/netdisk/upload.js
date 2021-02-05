export default {
  data () {
    return {
      isCreated: false,
      isCoverFiles: false, // 覆盖所有文件
      isJumpFiles: false, // 跳过所有文件
      isTimeStampFiles: false, // 文件名称中加一个时间戳
      isUploadFile: true, // 上传类型为文件
      uploadFileArray: [],
      fileList: [],
      allFileList: [],
      tabelSameFiles: [],
      dirParams: {},
      fileSymbol: 0,
      allUploadFiles: [],
      stopFlag: 1
    }
  },
  mixins: [],
  methods: {
    // 文件夹上传
    submintfiles (fileList) {
      const self = this
      var promise1 = null
      self.cur_progress = false
      self.uploadFileArray = []
      self.tabelSameFiles = []
      const bucketPath = []
      const curfiles = []
      for (const key in fileList.target.files) {
        curfiles.push(fileList.target.files[key])
      }
      document.getElementById('volume_networkBox_uploadOptFolder').value = null // 清除上次上传的值，避免下次上传同名文件夹不触发上传
      const filesArray = curfiles.slice(0, curfiles.length - 2)
      if (filesArray.length > 0) {
        let PrefixKey = ''
        PrefixKey = self.cur_buckets.slice(1, (self.cur_buckets.length))
        const bucketParams = {
          Bucket: self.cur_buckets[0],
          FileType: 'dir',
          Key: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + filesArray[0].webkitRelativePath.split(filesArray[0].name)[0].split('/')[0] : filesArray[0].webkitRelativePath.split(filesArray[0].name)[0].split('/')[0]
        }
        filesArray.forEach(v => {
          self.uploadFileArray.push({
            bucket: v.webkitRelativePath.split(v.name)[0],
            name: v.name,
            level: v.webkitRelativePath.split(v.name)[0].split('/').length - 1,
            size: v.size,
            stream: v,
            lastModified: v.lastModified
          })
        })
        if (self.files.length > 0) {
          let isUploadFolder = false
          self.files.forEach(item => {
            // 上传的文件夹已存在时
            if (item.name === filesArray[0].webkitRelativePath.split(filesArray[0].name)[0].split('/')[0] && item.type === 'folder') {
              // 获取当前文件夹中的所有内容
              isUploadFolder = true
              self.uploadOutVisible = true
              promise1 = new Promise(function (resolve, reject) {
                bucketPath.push(item)
                self.dirFoundFile(bucketPath, true)
                resolve(self.folderFiles)
              })
            }
          })
          // 同名文件（夹）同级对比
          promise1 && promise1.then(function (data) {
            setTimeout(function () {
              for (var i = 0; i < self.uploadFileArray.length; i++) {
                var obj = self.uploadFileArray[i]
                var isExist = false
                for (var j = 0; j < data.length; j++) {
                  var aj = data[j]
                  if (obj.bucket === aj.bucket && obj.level === aj.level && obj.name === aj.name) { // 同名文件夹，同一阶层，文件名称相同
                    self.tabelSameFiles.push(aj)
                    isExist = true
                    break
                  }
                }
              }
            }, 100)
          })
        }
        if (!self.uploadOutVisible) {
          self.creatUploadDir(bucketParams, self.uploadFileArray)
        } else {
          self.dirParams = bucketParams
        }
      }
    },
    // 文件上传
    uploadFile (fileinfo) {
      const self = this
      self.cur_progress = false
      self.object_upload_name = fileinfo.file.name
      const file = fileinfo.file
      self.tabelSameFiles = []
      self.uploadFileArray = []
      self.fileList = []
      self.uploadFileArray.push({
        bucket: '',
        name: file.name,
        level: 1,
        size: file.size,
        stream: file,
        lastModified: file.lastModified
      })
      if (self.files.length > 0) {
        self.files.forEach((item, idx) => {
          if (item.name === file.name && item.name !== 'folder') {
            self.uploadOutVisible = true
            self.tabelSameFiles.push(file)
            self.fileList.push({
              bucket: '',
              name: file.name,
              level: 1,
              size: file.size,
              stream: file,
              lastModified: file.lastModified
            })
          }
        })
      }
      if (self.uploadOutVisible) {
        self.isUploadFile = true
      } else {
        file.fileSymbol = new Date().getTime() + (++self.fileSymbol)
        self.upload_fileList.push({
          name: file.name,
          percentage: 0,
          lastloaded: 0,
          size: file.size,
          type: file.name.split('.')[file.name.split('.').length - 1],
          isSuccess: true,
          mtime: new Date(file.lastModified).Format('yyyy-MM-dd hh:mm:ss'),
          fileSymbol: file.fileSymbol
        })
        const params = {
          Bucket: self.cur_buckets.join('/'),
          Key: file.name, // 要上传的文件名称
          Body: file,
          FileType: file.name.split('.')[file.name.split('.').length - 1]
        }
        self.addUploadFile(file, params)
      }
    },
    // 拖拽上传
    dragUploadFile () {
      const self = this
      var promise1 = null
      const bucketPath = []
      const dirSameFlies = [] // 文件夹中的同名文件
      const fileSameFlies = [] // 同名文件
      self.uploadFileArray = [] // 拖拽的所有文件
      self.tabelSameFiles = [] // 拖拽的所有相同文件
      self.cur_progress = false
      const dragName = []
      const tableFileName = []
      if (self.dragFiles.length > 0) {
        self.dragFiles.forEach(v => {
          self.uploadFileArray.push({
            bucket: v.fullPath ? v.fullPath.split(v.name)[0] : '',
            name: v.name,
            level: v.fullPath ? v.fullPath.split(v.name)[0].split('/').length - 1 : 0,
            size: v.size,
            stream: v,
            type: v.name.split('.')[v.name.split('.').length - 1],
            lastModified: v.lastModified
          })
          dragName.push(v.fullPath ? v.fullPath.split('/')[0] : v.name)
        })
      }
      self.fileList = []
      if (self.files.length > 0) {
        self.files.forEach(item => {
          if (item.type === 'folder') {
            promise1 = new Promise(function (resolve, reject) {
              bucketPath.push(item)
              self.dirFoundFile(bucketPath, true)
              resolve(self.folderFiles)
            })
          } else {
            item.level = 0
            self.fileList.push(item)
          }
          tableFileName.push(item.name)
        })
      }
      // 同名文件（夹）同级对比
      if (promise1) {
        promise1.then(function (data) {
          setTimeout(function () {
            for (var i = 0; i < self.uploadFileArray.length; i++) {
              var obj = self.uploadFileArray[i]
              var isExist = false
              for (var j = 0; j < data.length; j++) {
                var aj = data[j]
                if (obj.bucket === aj.bucket && obj.level === aj.level && obj.name === aj.name) { // 同名文件夹，同一阶层，文件名称相同
                  dirSameFlies.push(obj)
                  isExist = true
                  break
                }
              }
            }
          }, 100)
        })
      } else {
        for (var i = 0; i < self.uploadFileArray.length; i++) {
          var obj = self.uploadFileArray[i]
          var isExist = false
          for (var j = 0; j < self.fileList.length; j++) {
            var aj = self.fileList[j]
            if (obj.bucket === aj.bucket && obj.name === aj.name) { // 同名文件
              dirSameFlies.push(obj)
              isExist = true
              break
            }
          }
        }
        self.tabelSameFiles = dirSameFlies
        self.tabelSameFiles.length > 0 && (self.uploadOutVisible = true)
        if (self.uploadOutVisible) {
          self.uploadFileArray.forEach(item => {
            if (item.type === 'folder' && item.level === 1) {
              self.isUploadFile = false
              let PrefixKey = ''
              PrefixKey = self.cur_buckets.slice(1, (self.cur_buckets.length))
              const bucketParams = {
                Bucket: self.cur_buckets[0],
                FileType: 'dir',
                Key: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + item.bucket.split('/')[0] : item.bucket.split('/')[0]
              }
              self.dirParams = bucketParams
            } else {
              self.isUploadFile = true
            }
          })
        }
      }
      // 同名文件（夹）同级对比
      promise1 && promise1.then(function (data) {
        setTimeout(function () {
          const allDragFiles = data.concat(self.fileList)
          for (var i = 0; i < self.uploadFileArray.length; i++) {
            var obj = self.uploadFileArray[i]
            var isExist = false
            for (var j = 0; j < allDragFiles.length; j++) {
              var aj = allDragFiles[j]
              if (obj.bucket === aj.bucket && obj.level === aj.level && obj.name === aj.name) { // 同名文件夹，同一阶层，文件名称相同
                dirSameFlies.push(obj)
                isExist = true
                break
              }
            }
          }
          self.tabelSameFiles = dirSameFlies
          self.tabelSameFiles.length > 0 && (self.uploadOutVisible = true)
          if (self.uploadOutVisible) {
            self.uploadFileArray.forEach(item => {
              if (item.type === 'folder' && item.level === 1) {
                self.isUploadFile = false
                let PrefixKey = ''
                PrefixKey = self.cur_buckets.slice(1, (self.cur_buckets.length))
                const bucketParams = {
                  Bucket: self.cur_buckets[0],
                  FileType: 'dir',
                  Key: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + item.bucket.split('/')[0] : item.bucket.split('/')[0]
                }
                self.dirParams = bucketParams
              } else {
                self.isUploadFile = true
              }
            })
          }
        }, 100)
      })
      let isSameFileName = false
      tableFileName.forEach(v => {
        dragName.forEach(k => {
          if (v === k) {
            isSameFileName = true
          }
        })
      })
      if (!isSameFileName) {
        const folderFileArray = []
        const fileArray = []
        self.uploadFileArray.forEach(item => {
          if (item.bucket !== '') {
            folderFileArray.push(item)
          } else {
            fileArray.push(item)
          }
        })
        if (folderFileArray.length > 0) {
          let PrefixKey = ''
          PrefixKey = self.cur_buckets.slice(1, (self.cur_buckets.length))
          const bucketParams = {
            Bucket: self.cur_buckets[0],
            FileType: 'dir',
            Key: PrefixKey.length > 0 ? PrefixKey.join('/') + '/' + folderFileArray[0].bucket.split('/')[0] : folderFileArray[0].bucket.split('/')[0]
          }
          self.creatUploadDir(bucketParams, self.uploadFileArray)
        }
        if (fileArray.length > 0) {
          fileArray.forEach(item => {
            item.fileSymbol = new Date().getTime() + (++self.fileSymbol)
            self.upload_fileList.push({
              name: item.name,
              percentage: 0,
              lastloaded: 0,
              size: item.size,
              type: item.name.split('.')[item.name.split('.').length - 1],
              isSuccess: true,
              mtime: new Date(item.lastModified).Format('yyyy-MM-dd hh:mm:ss'),
              fileSymbol: item.fileSymbol
            })
            const params = {
              Key: item.name, // 要上传的文件名称
              Body: item.stream,
              FileType: item.type,
              Bucket: self.cur_buckets.join('/') + '/' + item.bucket
            }
            self.addUploadFile(item, params)
          })
        }
      }
    },
    // 覆盖所有同名文件
    handleCoverFiles () {
      const self = this
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      if (self.isUploadFile) {
        upload()
      } else {
        self.awsS3Client.copyObject(self.dirParams, function (err) {
          if (err) {
            self.$message({
              message: err,
              type: 'error',
              duration: 1500
            })
          } else {
            upload()
          }
        })
      }
      function upload () {
        self.uploadFileArray.forEach(item => {
          item.fileSymbol = new Date().getTime() + (++self.fileSymbol)
          self.upload_fileList.push({
            name: item.name,
            percentage: 0,
            lastloaded: 0,
            size: item.size,
            type: item.name.split('.')[item.name.split('.').length - 1],
            isSuccess: true,
            mtime: new Date(item.lastModified).Format('yyyy-MM-dd hh:mm:ss'),
            fileSymbol: item.fileSymbol
          })
          const params = {
            Key: item.name, // 要上传的文件名称
            Body: item.stream,
            FileType: item.type,
            Bucket: self.cur_buckets.join('/') + '/' + item.bucket
          }
          self.addUploadFile(item, params)
        })
      }
    },
    // 跳过所有同名文件
    handleJumpFiles () {
      const self = this
      self.allFileList = []
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.allFileList = self.fileList.concat(self.folderFiles)
      if (self.isUploadFile) {
        upload()
      } else {
        self.awsS3Client.copyObject(self.dirParams, function (err) {
          if (err) {
            self.$message({
              message: err,
              type: 'error',
              duration: 1500
            })
          } else {
            upload()
          }
        })
      }
      function upload () {
        const diffUploadFiles = []
        for (var i = 0; i < self.uploadFileArray.length; i++) {
          var obj = self.uploadFileArray[i]
          var isExist = false
          for (var j = 0; j < self.allFileList.length; j++) {
            var aj = self.allFileList[j]
            if (obj.bucket === aj.bucket && obj.name === aj.name && obj.level === aj.level) {
              isExist = true
              break
            }
          }
          if (!isExist) {
            diffUploadFiles.push(obj)
          }
        }
        if (diffUploadFiles.length > 0) {
          diffUploadFiles.forEach(item => {
            item.fileSymbol = new Date().getTime() + (++self.fileSymbol)
            self.upload_fileList.push({
              name: item.name,
              percentage: 0,
              lastloaded: 0,
              size: item.size,
              type: item.name.split('.')[item.name.split('.').length - 1],
              isSuccess: true,
              mtime: new Date(item.lastModified).Format('yyyy-MM-dd hh:mm:ss'),
              fileSymbol: item.fileSymbol
            })
            const params = {
              Key: item.name, // 要上传的文件名称
              Body: item.stream,
              FileType: item.type,
              Bucket: self.cur_buckets.join('/') + '/' + item.bucket
            }
            self.addUploadFile(item, params)
          })
        } else {
          self.uploadOutVisible = false
        }
      }
    },
    // 所有同名文件加时间戳
    handleTimeStampFiles () {
      const self = this
      self.allFileList = []
      self.allFileList = self.fileList.concat(self.folderFiles)
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      if (self.isUploadFile) {
        upload()
      } else {
        self.awsS3Client.copyObject(self.dirParams, function (err) {
          if (err) {
            self.$message({
              message: err,
              type: 'error',
              duration: 1500
            })
          } else {
            upload()
          }
        })
      }
      function upload () {
        self.uploadFileArray.forEach(item => {
          const params = {
            Key: item.name, // 要上传的文件名称
            Body: item.stream,
            FileType: item.type,
            Bucket: self.cur_buckets.join('/') + '/' + item.bucket
          }
          self.allFileList.forEach(v => {
            if (v) {
              if (item.bucket === v.bucket && item.name === v.name && item.level === v.level) {
                params.Key = item.name.split('.').slice(0, (item.name.split('.').length - 1)).join('.') + '-' + new Date().Format('yyyy-MM-dd-hh:mm:ss') + '.' + item.name.split('.')[item.name.split('.').length - 1]
              }
              if (item.type !== 'folder' && v.bucket === '' && item.name === v.name) {
                params.Key = item.name.split('.').slice(0, (item.name.split('.').length - 1)).join('.') + '-' + new Date().Format('yyyy-MM-dd-hh:mm:ss') + '.' + item.name.split('.')[item.name.split('.').length - 1]
              }
            }
          })
          item.fileSymbol = new Date().getTime() + (++self.fileSymbol)
          self.upload_fileList.push({
            name: params.Key,
            percentage: 0,
            lastloaded: 0,
            size: item.size,
            type: item.name.split('.')[item.name.split('.').length - 1],
            isSuccess: true,
            mtime: new Date(item.lastModified).Format('yyyy-MM-dd hh:mm:ss'),
            fileSymbol: item.fileSymbol
          })
          self.addUploadFile(item, params)
        })
      }
    },
    // 创建文件夹并上传文件
    creatUploadDir (bucketParams, filesArray) {
      const self = this
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.awsS3Client.copyObject(bucketParams, function (err) {
        if (err) {
          self.$message({
            message: err && err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
            type: 'error',
            duration: 1500
          })
        } else {
          filesArray.forEach(item => {
            item.fileSymbol = new Date().getTime() + (++self.fileSymbol)
            self.upload_fileList.push({
              name: item.name,
              percentage: 0,
              lastloaded: 0,
              size: item.size,
              type: item.name.split('.')[item.name.split('.').length - 1],
              isSuccess: true,
              mtime: new Date(item.lastModified).Format('yyyy-MM-dd hh:mm:ss'),
              fileSymbol: item.fileSymbol
            })
            const params = {
              Key: item.name, // 要上传的文件名称
              Body: item.stream,
              FileType: item.type,
              Bucket: self.cur_buckets.join('/') + '/' + item.bucket
            }
            self.addUploadFile(item, params)
          })
        }
      })
    },
    // 添加上传文件到列表
    addUploadFile (file, params) {
      this.allUploadFiles.push({ file, params })
      this.readyUploadFile()
    },
    // 整理判断上传的下一个文件
    async readyUploadFile () {
      const self = this
      await self.refreshStorageUseData()
      let curUploadFileSymbol = ''
      let len = self.upload_fileList.length
      for (let i = 0; i < len; i++) {
        if (self.upload_fileList[i].isSuccess && self.upload_fileList[i].lastloaded > 0 && self.upload_fileList[i].lastloaded < self.upload_fileList[i].size) {
          return
        }
        if (i > 0 && (self.upload_fileList[i - 1].percentage === 100 || !self.upload_fileList[i - 1].isSuccess) && !self.upload_fileList[i].isStartUplod) {
          self.upload_fileList[i].isStartUplod = true
          curUploadFileSymbol = self.upload_fileList[i].fileSymbol
          break
        }
        if (i === 0 && !self.upload_fileList[0].isStartUplod) {
          self.upload_fileList[0].isStartUplod = true
          curUploadFileSymbol = self.upload_fileList[0].fileSymbol
          break
        }
      }
      len = self.allUploadFiles.length
      for (let i = 0; i < len; i++) {
        if (self.allUploadFiles[i].file.fileSymbol === curUploadFileSymbol) {
          self.startUploadFile(self.allUploadFiles[i].file, self.allUploadFiles[i].params)
          break
        }
      }
    },
    // 上传文件
    startUploadFile (file, params) {
      const self = this
      self.uploadOutVisible = false
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      if (self.upload.used + file.size / 1024 > self.upload.total) {
        self.upload_fileList.forEach(item => {
          if (item.name === file.name) {
            item.isSuccess = false
          }
        })
        // self.s3UploadRequest = null
        self.$message({
          message: self.$t('disk.noEnoughStorageSpace'),
          type: 'error',
          duration: 1500
        })
        return
      }
      let curBucketObj = {}
      self.curBucketObjList.forEach(item => {
        if (params.Bucket.split('/')[0] === item.name) {
          curBucketObj = item
        }
      })
      if (curBucketObj.StorageQuota != 0 && curBucketObj.byteNum + file.size > Number(curBucketObj.StorageQuota)) {
        self.upload_fileList.forEach(item => {
          if (item.name === file.name) {
            item.isSuccess = false
          }
        })
        setTimeout(() => {
          self.$message({
            message: self.$t('disk.noEnoughBucketQuotaSpace') + file.name + self.$t('disk.uploadFailed'),
            type: 'error',
            duration: 1500
          })
        }, 500)
        self.readyUploadFile() // 上传下一个文件
        return
      }
      self.uploadErr = false
      if (file.size <= 64 * 1024 * 1024) {
        self.s3UploadRequest = self.awsS3Client.putObject(params, function (err, data) {
          if (err) {
            if (err.code === 'SignatureDoesNotMatch') {
              self.upload_fileList.forEach(item => {
                if (item.name === file.name) {
                  item.lastloaded = item.size
                  item.percentage = 100
                }
              })
              return false
            } else {
              self.upload_fileList.forEach(item => {
                if (item.name === file.name) {
                  item.isSuccess = false
                }
              })
            }
            if (err.message !== `Cannot set property 'percentage' of undefined`) {
              if (err.message === 'Access Denied.') {
                self.$message({
                  message: self.$t('disk.noPermission'),
                  type: 'error',
                  duration: 1500
                })
                self.uploadErr = true
                self.uploadLoading = false
              } else {
                self.$message({
                  message: err,
                  type: 'error',
                  duration: 1500
                })
              }
            }
          }
          if (file.size < 10) {
            self.upload_fileList.forEach(item => {
              if (item.fileSymbol === file.fileSymbol) {
                item.percentage = 100
                item.lastloaded = file.size
              }
              self.readyUploadFile() // 上传下一个文件
              self.cur_progress = self.upload_fileList.every(item => {
                return (item.percentage === 100 || !item.isSuccess) // 所有item都成立
              })
            })
          }
        })
      } else {
        self.s3UploadRequest = self.awsS3Client.upload(params, function (err, data) {
          if (err) {
            self.upload_fileList.forEach(item => {
              item.isSuccess = false
            })
            if (err.message !== `Cannot set property 'percentage' of undefined`) {
              if (err.message === 'Access Denied.') {
                self.$message({
                  message: self.$t('disk.noPermission'),
                  type: 'error',
                  duration: 1500
                })
                self.uploadErr = true
                self.uploadLoading = false
              } else {
                self.$message({
                  message: err,
                  type: 'error',
                  duration: 1500
                })
              }
            }
          }
        })
      }
      self.s3UploadRequest.on('httpUploadProgress', function (progress, response) {
        const progressLoaded = parseInt(progress.loaded)
        const progressTotal = parseInt(progress.total)
        if (progress && progress.key) {
          self.upload_fileList.forEach(item => {
            if (item.name === (progress && progress.key.split('/')[progress.key.split('/').length - 1])) {
              if (item.lastloaded < progressLoaded) {
                item.percentage = parseInt((progressLoaded / progressTotal) * 100)
                self.upload.used += (progressLoaded - item.lastloaded) / 1024
                self.upload.percentage = self.upload.used / self.upload.total
                item.lastloaded = progressLoaded
              } else {
                if (item.lastloaded === progressLoaded && self.upload.percentage > 0.99) {
                  item.isSuccess = false
                  self.s3UploadRequest = null
                  self.$message({
                    message: self.$t('disk.noEnoughStorageSpace'),
                    type: 'error',
                    duration: 1500
                  })
                }
              }
            }
          })
        } else if (response && response.request) {
          self.upload_fileList.forEach(item => {
            if (item.name === (response && response.request.params.Key.split('/')[response.request.params.Key.split('/').length - 1])) {
              if (parseInt(item.lastloaded) === 0) {
                item.percentage = parseInt((progressLoaded / progressTotal) * 100)
                item.lastloaded = progressLoaded
              } else if (item.lastloaded < progressLoaded) {
                item.percentage = parseInt((progressLoaded / progressTotal) * 100)
                item.lastloaded = progressLoaded
              }
            }
          })
        }
        if (self.stopFlag == 2) {
          self.readyUploadFile() // 上传下一个文件
          self.stopFlag = 1
          return false
        }
        if (parseInt((progressLoaded / progressTotal) * 100) >= 100) {
          self.readyUploadFile() // 上传下一个文件
          self.cur_progress = self.upload_fileList.every(item => {
            return item.percentage === 100 // 所有item都成立
          })
        }
      })
      self.reqList.push(self.s3UploadRequest)
      self.s3UploadRequest.send(function (err, data) {
        if (err) {
          self.uploadErrCb(err, data)
        }
      })
      self.isShowUpload = true
    },
    stopUpload (row) {
      this.upload_fileList.forEach((item, index) => {
        if (item.name == row.name) {
          this.upload_fileList.splice(index, 1)
        }
      })
      this.allUploadFiles.forEach((item, index) => {
        if (item.name == row.name) {
          this.allUploadFiles.splice(index, 1)
        }
      })
      this.stopFlag = 2
      this.readyUploadFile()
    },
    // 递归查找该文件夹下的所有内容（包括文件、文件夹）
    dirFoundFile (bucketPath, isFirstLevel) {
      const self = this
      let params = {}
      // self.folderFiles = []
      let tablePrefixKey = ''
      let dirPath = ''
      if (isFirstLevel) {
        tablePrefixKey = self.cur_buckets.slice(1, (self.cur_buckets.length))
        bucketPath.forEach(item => {
          dirPath = item.name
        })
        params = {
          Bucket: self.cur_buckets[0],
          Prefix: tablePrefixKey.length > 0 ? tablePrefixKey.join('/') + '/' + dirPath + '/' : tablePrefixKey.join('/') + dirPath + '/'
        }
      } else {
        bucketPath.forEach(item => {
          tablePrefixKey = item
        })
        params = {
          Bucket: self.cur_buckets[0],
          Prefix: tablePrefixKey
        }
      }
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.awsS3Client.listObjects(params, function (err, data) {
        if (err) {
          self.$message({
            message: err && err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
            type: 'error',
            duration: 1500
          })
        } else {
          data.Contents.forEach(item => {
            self.folderFiles.push({
              bucket: data.Prefix,
              name: item.Key,
              level: data.Prefix ? data.Prefix.split('/').length - 1 : 1,
              size: item.Size,
              type: item.Key.split('.')[item.Key.split('.').length - 1] || ''
            })
          })
          self.folderFiles = self.folderFiles.sort(function (a, b) {
            return a.size - b.size
          })
          // 数组去重
          for (var i = 0; i < self.folderFiles.length - 1; i++) {
            if (self.folderFiles[i].bucket === self.folderFiles[i + 1].bucket && self.folderFiles[i].name === self.folderFiles[i + 1].name) {
              self.folderFiles.splice(i, 1)
              i--
            }
          }
          data.CommonPrefixes.forEach(item => {
            bucketPath.push(item.Prefix)
            self.dirFoundFile(bucketPath, false)
          })
        }
      })
    },
    cancelOpt () {
      this.uploadFileArray = []
      this.fileList = []
      this.allFileList = []
      this.tabelSameFiles = []
    },
    uploadErrCb (err) {
      const self = this
      if (err.code === 'XTMSQuotaExceeded' || err.code === 'AccessDenied') {
        const length = self.upload_fileList.length
        for (let i = 0; i < length; i++) {
          if (self.upload_fileList[i].lastloaded < self.upload_fileList[i].size && self.upload_fileList[i].isStartUplod) {
            self.upload_fileList[i].isSuccess = false
          }
        }
        if (err.code === 'AccessDenied') {
          self.$message({
            message: self.$t('disk.noPermission'),
            type: 'error',
            duration: 1500
          })
        } else {
          self.$message({
            message: err.message,
            type: 'error',
            duration: 1500
          })
        }
        self.readyUploadFile()
      }
    }
  }
}

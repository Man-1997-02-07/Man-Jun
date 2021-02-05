export default {
  data () {
    return {

    }
  },
  mixins: [],
  methods: {
    // 删除文件夹或者对象
    handleNetworkBoxDel (row) {
      if (row.access == 'READ') {
        this.$message({
          message: this.$t('disk.noPermission'),
          type: 'error',
          duration: 3000
        })
        return false
      }
      const self = this
      self.$Api.system.updateWsToken()
      self.$confirm(this.$t('disk.delTip'), this.$t('resource.tip'), {
        confirmButtonText: this.$t('resource.confirm'),
        type: 'warning'
      }).then(() => {
        if (!self.isShow && self.getFilePath(row.path) === this.cur_buckets.join('/')) {
          // 删除文件夹
          self.awsS3Client.deleteBucket({ Bucket: row.name }, function (err) {
            if (err) {
              const errTxt = {
                BucketNotEmpty: self.$t('disk.cannotDel')
              }
              // console.log(JSON.stringify(err))
              self.$message({
                message: errTxt[err.code] || err,
                type: 'error',
                duration: 1500
              })
            } else {
              self.$message({
                message: this.$t('disk.delSuccess'),
                type: 'success',
                duration: 1500
              })
              self.getNetworkDiskInfo()
              self.isNewFiles = false
            }
          })
        } else {
          // 删除文件
          const params = {
            Bucket: self.getFilePath(row.path),
            Key: row.type === 'folder' ? row.name + '/' : row.name
          }
          self.awsS3Client.deleteObject(params, function (err, data) {
            ;
            if (err) {
              self.$message({
                message: err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
                type: 'error',
                duration: 1500
              })
            } else {
              self.$message({
                // message: `删除文件夹${self.cur_buckets.join('/')}中的文件${row.name}成功！`,
                message: self.$t('disk.delSuccess'),
                type: 'success',
                duration: 1500
              })
              if (self.isShow) {
                self.getBucketObject()
              } else {
                self.getNetworkDiskInfo()
                self.isNewFiles = false
              }
            }
          })
        }
      })
    },
    // 批量删除文件
    handleNetworkBoxDelMultiple () {
      const self = this
      self.$confirm(this.$t('disk.delTip'), this.$t('resource.tip'), {
        confirmButtonText: this.$t('resource.confirm'),
        type: 'warning'
      }).then(() => {
        // 每次请求前刷新token失效时间
        self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
        // 删除文件夹
        self.setState({
          attr: 'onLoading',
          val: true
        })
        let num = 0
        const error_names = []

        let params = {}
        if (!self.isShow) {
          self.networkDataSelect.forEach(item => {
            if (self.getFilePath(item.path) === this.cur_buckets.join('/')) { delFolder(item) } else { delFiles(item) }
          })
        } else {
          self.networkDataSelect.forEach(item => {
            delFiles(item)
          })
        }
        // 删除文件夹
        function delFolder (row) {
          self.awsS3Client.deleteBucket({ Bucket: row.name }, function (err) {
            self.setState({
              attr: 'onLoading',
              val: false
            })
            num++
            if (err) {
              error_names.push(row.name)
              deleteCallback(err)
            } else {
              deleteCallback(row)
            }
          })
        }
        // 删除文件
        function delFiles (row) {
          params = {
            Bucket: self.getFilePath(row.path),
            Key: row.type === 'folder' ? row.name + '/' : row.name
          }
          self.awsS3Client.deleteObject(params, function (err) {
            self.setState({
              attr: 'onLoading',
              val: false
            })
            num++
            if (err) {
              error_names.push(row.name)
              deleteCallback(err)
            } else {
              deleteCallback(row)
            }
          })
        }

        function deleteCallback (err) {
          self.$nprogress.done()
          self.setState({
            attr: 'onLoading',
            val: false
          })
          if (error_names.length > 0) {
            const h = self.$createElement
            const error_ms = []
            let err_ms = ''
            const show_err_ms = []
            error_names.forEach((item) => {
              if (err.message && err.message.includes('Access Denied')) {
                err_ms = item + self.$t('disk.noPermission')
              } else {
                err_ms = item + self.$t('disk.delFailed')
              }
              error_ms.push(err_ms)
            })
            for (const i in error_ms) {
              show_err_ms.push(h('p', null, error_ms[i]))
            };
            self.$elMessage({
              message: h('p', null, show_err_ms),
              type: 'error'
            })
          } else {
            if (num === self.networkDataSelect.length) {
              self.$message({
                message: self.$t('disk.delSuccess'),
                type: 'success'
              })
            }
          }
          if (self.isShow && num === self.networkDataSelect.length) {
            self.getBucketObject()
          } else if (!self.isShow && num === self.networkDataSelect.length) {
            self.getNetworkDiskInfo()
            self.isNewFiles = false
          }
        }
      })
    }
  }
}

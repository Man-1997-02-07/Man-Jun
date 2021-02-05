export default {
  data () {
    return {
      sourceDirList: [], // 源路径中的同名文件夹下的所有文件
      sourceFile: [], // 源路径中的文件
      operateDirList: [], // 源路径中的同名文件夹下的所有文件,
      operateFile: [], // 批量移动中的文件
      sourceAllFile: [],
      operateAllFile: [],
      tabelSameFiles: [],
      operateType: '',
      curSelectOptObj: []
    }
  },
  mixins: [],
  methods: {
    // 获取源路径文件夹中的所有文件
    getSourceDirList () {
      const self = this
      const moveSourceDir = [] // 源路径中的文件夹
      self.sourceFile = []
      let diaFiles = [] // 目標文件夾下的文件列表
      if (self.operateType === 'copy') {
        diaFiles = self.copyDiaFiles
      } else {
        diaFiles = self.moveDiaFiles
      }
      diaFiles.forEach(item => {
        if (item.Prefix) {
          moveSourceDir.push(item)
          self.sourceFile.push({
            name: item.Prefix.slice(0, item.Prefix.length - 1),
            Prefix: item.Prefix
          })
        } else {
          self.sourceFile.push({
            bucket: '',
            name: item.Key,
            level: 1,
            size: item.Size,
            type: item.Key.split('.')[item.Key.split('.').length - 1] || ''
          })
        }
      })
      // 获取源路径同名文件夹中的所有文件
      // moveSourceDir.forEach(item => {
      //   const bucketPath = []
      //   bucketPath.push(item.Prefix.split('/')[0])
      //   self.dirFoundFile(bucketPath, true, 'source')
      // })
    },
    // 获取批量移动（复制）操作的文件夹中的所有文件
    getOptDirList () {
      const self = this
      const operateDir = [] // 批量移动中的文件
      if (self.copyType === 1) {
        operateDir.push(self.copyFileObj)
      } else if (self.moveType === 1) {
        operateDir.push(self.moveFileObj)
      } else {
        self.operateFile = []
        self.curSelectOptObj = []
        self.batchSelectObj.forEach(item => {
          self.curSelectOptObj.push({
            bucket: item.bucket,
            name: item.name,
            size: item.size,
            type: item.type,
            path: self.getFilePath(item.path)
          })
          // if (item.type !== 'folder') {
          self.operateFile.push({
            bucket: '',
            name: item.name,
            level: 1,
            size: item.size,
            type: item.type,
            path: self.getFilePath(item.path)
          })
          // } else {
          //   operateDir.push(item)
          // }
        })
      }
      // 获取批量移动中的文件夹中的所有文件
      // operateDir.forEach(item => {
      //   const bucketPath = []
      //   bucketPath.push(item.name)
      //   self.dirFoundFile(bucketPath, true, 'operate')
      // })
    },
    // 获取文件夹路径
    getFilePath (path) {
      if (!path) return this.cur_buckets.join('/')
      if (path.charAt(path.length - 1) === '/') return path.split('/').splice(1, path.split('/').length - 3).join('/')
      else return path.split('/').splice(1, path.split('/').length - 2).join('/')
    },
    // 确认复制操作
    confirmCopy () {
      this.confirmOprateFile(1)
    },
    // 确认移动操作
    confirmMoveFile () {
      this.confirmOprateFile(2)
    },
    // 覆盖所有同名文件
    coverAllFilesOpt () {
      const self = this
      // if (self.operateType === 'copy') {
      let params = {}
      const Bucket = self.bucketsPath.join('/')
      const oprateFileObj = self[self.operateType === 'copy' ? 'copyFileObj' : 'moveFileObj']
      if (self[self.operateType === 'copy' ? 'copyType' : 'moveType'] === 1) {
        params = {
          Bucket: Bucket,
          Key: oprateFileObj.type === 'folder' ? '' : oprateFileObj.name,
          CopySource: `/${encodeURI(self.getFilePath(oprateFileObj.path))}/${encodeURI(oprateFileObj.name)}`
        }
      } else {
        const BatchSource = []
        self.curSelectOptObj.forEach(item => {
          BatchSource.push('/' + encodeURI(item.path) + '/' + encodeURI(item.name))
        })
        params = {
          Bucket: Bucket,
          Key: '',
          BatchSource: BatchSource.toString()
        }
      }
      self.netDiskOperate(params, self.operateType)

      self.$emit('getNetDiskFilesOpt', { netDiskOpttVisible: false, sameObject: 0 })
    },
    // 跳过所有同名文件
    jumpAllFilesOpt () {
      const self = this
      // if (self.operateType === 'copy') {
      let params = {}
      const Bucket = self.bucketsPath.join('/')
      if (self[self.operateType === 'copy' ? 'copyType' : 'moveType'] !== 1) {
        const diffUploadFiles = []
        for (var i = 0; i < self.operateAllFile.length; i++) {
          var obj = self.operateAllFile[i]
          var isExist = false
          for (var j = 0; j < self.sourceAllFile.length; j++) {
            var aj = self.sourceAllFile[j]
            if (aj.name === obj.name) {
              isExist = true
              break
            }
          }
          if (!isExist) {
            diffUploadFiles.push(obj)
          }
        }
        const BatchSource = []
        diffUploadFiles.forEach(item => {
          BatchSource.push('/' + encodeURI(item.path) + '/' + encodeURI(item.name))
        })
        const lastParam = self.operateType === 'copy' ? 'copy' : 'move'
        params = {
          Bucket: Bucket,
          Key: '',
          BatchSource: BatchSource.toString()
        }
        diffUploadFiles.length > 0 && self.netDiskOperate(params, lastParam)
      }
      self.operateType === 'copy' ? self.cancelCopy() : self.cancelMoveFile()
      self.$emit('getNetDiskFilesOpt', { netDiskOpttVisible: false, sameObject: 0 })
    },
    // 网盘文件操作（移动、复制）
    netDiskOperate (params, type) {
      const self = this
      // self.setState({
      //   attr: 'innerLoading',
      //   val: true
      // })
      // 判断是移动还是复制
      if (type === 'move' && params.BatchSource) {
        params.MoveBatchSource = params.BatchSource
      } else if (type === 'copy' && params.BatchSource) {
        params.CopyBatchSource = params.BatchSource
      }
      delete params.BatchSource

      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      self.awsS3Client.copyObject(params, function (err, data) {
        self.setState({
          attr: 'innerLoading',
          val: false
        })
        if (err) {
          self.$message({
            message: err.message.includes('Access Denied') ? self.$t('disk.noPermission') : err,
            type: 'error',
            duration: 1500
          })
          self.bucketsPath = []
        } else {
          if (type === 'move') {
            self.$message({
              message: self.$t('disk.operationSuccess'),
              type: 'success',
              duration: 1500
            })
            self.cancelMoveFile()
          } else {
            self.$message({
              message: self.$t('disk.fileCopySuccess'),
              type: 'success',
              duration: 1500
            })
            self.cancelCopy()
          }
          if (self.cur_buckets.length === 0) {
            self.$emit('updateDiskInfo')
          } else {
            self.$emit('updateBucketObject', self.bucketsPath, false)
          }
        }
      })
    },
    // 递归查找该文件夹下的所有内容（包括文件、文件夹）
    dirFoundFile (bucketPath, isFirstLevel, type) {
      const self = this
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      let params = {}
      let tablePrefixKey = ''
      let dirPath = ''
      if (isFirstLevel) {
        tablePrefixKey = type === 'source' ? self.bucketsPath.slice(1, (self.bucketsPath.length)) : self.cur_buckets.slice(1, (self.cur_buckets.length))
        bucketPath.forEach(item => {
          dirPath = item
        })
        params = {
          Bucket: type === 'source' ? self.bucketsPath[0] : self.cur_buckets[0],
          Prefix: tablePrefixKey.length > 0 ? tablePrefixKey.join('/') : tablePrefixKey.join('/') + dirPath + '/'
        }
      } else {
        bucketPath.forEach(item => {
          tablePrefixKey = item
        })
        params = {
          Bucket: type === 'source' ? self.bucketsPath[0] : self.cur_buckets[0],
          Prefix: tablePrefixKey
        }
      }
      self.awsS3Client.listObjects(params, function (err, data) {
        if (err) {
          self.$message({
            message: err,
            type: 'error',
            duration: 1500
          })
        } else {
          data.Contents.forEach(item => {
            self[type === 'source' ? 'sourceDirList' : 'operateDirList'].push({
              bucket: data.Prefix,
              name: item.Key,
              level: data.Prefix ? data.Prefix.split('/').length - 1 : 1,
              size: item.Size,
              type: item.Key.split('.')[item.Key.split('.').length - 1] || ''
            })
          })
          const lastParam = type === 'source' ? 'source' : 'operate'
          data.CommonPrefixes.forEach(item => {
            bucketPath.push(item.Prefix)
            self.dirFoundFile(bucketPath, false, lastParam)
          })
        }
      })
    },
    confirmOprateFile (optType) {
      const oprateType = ['', 'copyType', 'moveType']
      const self = this
      const optBucket = self.bucketsPath.join('/') // 目標文件夾
      self.operateAllFile = self.operateFile.concat(self.operateDirList)
      const oprateFileObj = self[optType === 1 ? 'copyFileObj' : 'moveFileObj']
      var promise1 = new Promise(function (resolve, reject) {
        self.getSourceDirList()
        resolve(self.sourceDirList)
      })
      promise1.then(function (data) {
        setTimeout(function () {
          self.sourceAllFile = self.sourceFile.concat(data)
          if (self.sourceAllFile.length > 0) {
            compareFiles()
          } else {
            readyRequest()
          }
          // 对比取相同的文件
          function compareFiles () {
            let isExist = false
            self.tabelSameFiles.length = 0
            if (self[oprateType[optType]] !== 1) {
              for (let i = 0; i < self.operateAllFile.length; i++) {
                const obj = self.operateAllFile[i]
                // let isExist = false
                for (let j = 0; j < self.sourceAllFile.length; j++) {
                  const aj = self.sourceAllFile[j]
                  // 对比方式不同（因为单传时bucket字段不一样）
                  if (aj.name === obj.name) { // 文件名称相同
                    self.tabelSameFiles.push(aj)
                    isExist = true
                  }
                }
              }
              if (isExist) { self.$emit('getNetDiskFilesOpt', { netDiskOpttVisible: true, type: optType, optObject: self.operateAllFile.length, sameObject: self.tabelSameFiles.length }) }
            } else {
              self.tabelSameFiles.length = 0
              self.sourceAllFile.forEach(item => {
                if (item.name === oprateFileObj.name) {
                  self.tabelSameFiles.push(item)
                  isExist = true
                }
              })
              if (isExist) { self.$emit('getNetDiskFilesOpt', { netDiskOpttVisible: true, type: optType, optObject: 1, sameObject: self.tabelSameFiles.length }) }
            }
            if (!isExist) {
              readyRequest()
            }
          }
          // 整理准备請求
          function readyRequest () {
            const BatchSource = []
            if (self[oprateType[optType]] === 1) {
              BatchSource.push(`/${encodeURI(self.getFilePath(oprateFileObj.path))}/${encodeURI(oprateFileObj.name)}`)
            } else {
              self.curSelectOptObj.forEach(item => {
                BatchSource.push('/' + encodeURI(item.path) + '/' + encodeURI(item.name))
              })
            }
            const params = {
              Bucket: optBucket,
              Key: '',
              BatchSource: BatchSource.toString()
            }
            self.netDiskOperate(params, self.operateType)
          }
        }, 100)
      })
    }
  }
}

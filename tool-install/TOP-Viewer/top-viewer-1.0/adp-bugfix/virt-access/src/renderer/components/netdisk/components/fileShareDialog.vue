<template>
  <el-dialog
    id="volume_netDisk--dialog-share"
    :visible.sync="visible"
    :close-on-click-modal="false"
    width="550px"
    @close="cancelShare">
    <template slot="title">
      <el-tooltip
        v-if="shareFiles.name && shareFiles.name.length > 25"
        class="item"
        effect="light"
        :content="`${$t('disk.shareFile')}`+`${shareFiles.name}`"
        placement="top">
        <span class="dialog_title">{{ $t('disk.shareFile') }}{{ `${shareFiles.name}` }}</span>
      </el-tooltip>
      <span
        v-else
        class="dialog_title">{{ $t('disk.shareFile') }}{{ `${shareFiles.name}` }}</span>
    </template>
    <div v-if="!isShareHttp">
      <label>{{ $t('disk.validate') }}</label>
      <el-select
        id="volume_netDisk--dialog-share--slect"
        v-model="cur_timeLimit"
        style="margin-left: 10px;width: 260px;"
        :placeholder="$t('disk.selectValidate')">
        <el-option
          v-for="(item,index) in timeLimit"
          :key="index"
          :label="item.label"
          :value="item.value" />
      </el-select>{{ $t('disk.days') }}
    </div>
    <div v-else>
      <span>{{ $t('disk.pLink') }}
        <span style="color:#3583E3">({{ cur_timeLimit/(24*60*60) }}{{ $t('disk.daysOut') }})</span>
        <el-input
          id="volume_netDisk--dialog-share--http"
          v-model="shareFiles.http"
          style="margin-top: 20px;"
          type="textarea"
          :autosize="true" />
      </span>
    </div>
    <div slot="footer">
      <el-button
        v-if="!isShareHttp"
        id="volume_netDisk--dialog-share--urlCreat"
        type="primary"
        :disabled="innerLoading"
        @click="creatSharelink">
        {{ $t('disk.createLink') }}
      </el-button>
      <el-button
        v-if="isShareHttp"
        id="volume_netDisk--dialog-share--urlCopy"
        type="primary"
        @click="handleHttpCopy">
        {{ $t('disk.copyLink') }}
      </el-button>
      <el-button
        v-if="!isShareHttp"
        id="volume_netDisk--dialog-share--cancel"
        class="close-button"
        @click="cancelShare">
        {{ $t('resource.cancel') }}
      </el-button>
      <el-button
        v-if="isShareHttp"
        id="volume_netDisk--dialog-share--close"
        class="close-button"
        @click="cancelShare">
        {{ $t('resource.close') }}
      </el-button>
    </div>
    <vs-loading
      :is-show="innerLoading"
      class-name="vs-inner-loading" />
  </el-dialog>
</template>

<script>
export default {
  components: {

  },
  props: {
    dialogShareVisible: {
      type: Boolean,
      default: false
    },
    isShow: {
      type: Boolean,
      default: false
    },
    awsS3Client: {
      type: Object
    },
    shareFiles: {
      type: Object,
      default: {}
    },
    cur_buckets: Array,
    getFilePath: {
      type: Function
    },
    curClusterUuid: {
      type: String
    }
  },
  data () {
    return {
      visible: this.dialogShareVisible,
      isShareHttp: false,
      cur_timeLimit: '',
      timeLimit: [
        { label: '1', value: 24 * 60 * 60 },
        { label: '3', value: 3 * 24 * 60 * 60 },
        { label: '5', value: 5 * 24 * 60 * 60 },
        { label: '7', value: 7 * 24 * 60 * 60 }
      ],
      s3ShareRequest: null
    }
  },
  computed: {

  },
  watch: {
    dialogShareVisible: function (newval) {
      this.visible = newval
    }
  },
  created () {

  },
  mounted () {
    this.cur_timeLimit = ''
  },
  methods: {
    // 创建分享连接
    creatSharelink () {
      const self = this
      if (!self.cur_timeLimit) {
        self.$message({
          message: self.$t('disk.selectValidate'),
          type: 'error',
          duration: 1500
        })
        return
      }
      let netDiskInfo = null
      self.$Api.system.updateWsToken({ cluster_uuid: self.cluster_uuid || self.curClusterUuid })
      if ((typeof self.netDisk) === 'string') {
        netDiskInfo = JSON.parse(self.netDisk)
      } else {
        netDiskInfo = self.netDisk
      }
      const params = {
        Bucket: self.getFilePath(self.shareFiles.path),
        Key: self.shareFiles.name,
        Expires: String(self.cur_timeLimit)
      }
      self.s3ShareRequest = self.awsS3Client.headObject(params, function (err, data) {
        if (err) {
          self.$message({
            message: self.$t('disk.shareError') + err,
            type: 'error',
            duration: 1500
          })
        } else {
          self.isShareHttp = true
        }
      })
      self.s3ShareRequest.on('httpHeaders', function (statusCode, headers) {
        for (const key in headers) {
          if (key === 'x-amz-share-uuid') {
            self.shareFiles.http = `${netDiskInfo.useSSL}://${netDiskInfo.endPoint}:${parseInt(netDiskInfo.port)}/${self.shareFiles.name}?uuid=${headers[key]}`
          }
        }
      })
    },
    // 取消分享
    cancelShare () {
      this.$emit('update:dialogShareVisible', false)
      this.$emit('update:shareFiles', {})
      this.isShareHttp = false
      this.cur_timeLimit = ''
    },
    handleHttpCopy () {
      var textarea = document.getElementById('volume_netDisk--dialog-share--http')
      textarea.select()
      document.execCommand('copy')
    }
  }
}
</script>

<style scoped lang="scss">
  .dialog_title{
    font-size: 18px;
    line-height: 24px;
    width: calc(100% - 100px);
    display: inline-block;
    vertical-align: middle;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }
</style>

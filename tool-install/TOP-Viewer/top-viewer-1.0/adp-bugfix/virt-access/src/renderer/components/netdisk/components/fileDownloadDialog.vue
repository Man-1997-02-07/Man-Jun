<template>
  <el-dialog
    id="volume_netDisk--dialog-download"
    :title="$t('disk.downloadFiles')"
    :visible.sync="visible"
    :close-on-click-modal="false"
    :show-close="true"
    width="685px"
    @close="cancelDownload">
    <div>
      <div style="margin-bottom:10px;">
        <img
          class="netDisk-icon"
          src="../../../assets/images/netdisk/wenjianjia.png">
        <span>{{ $t('disk.downloadFilesName') }}</span>
      </div>
      <el-form
        ref="form"
        label-width="80px">
        <el-form-item :label="$t('disk.downloadTo')">
          <el-row>
            <el-col :span="21">
              <el-input v-model="path" />
            </el-col>
            <el-col :span="3">
              <div
                style="display: inline-block; width: 45px; height: 30px; margin-left: 16px; position: relative">
                <input
                  type="file"
                  name="file"
                  directory
                  webkitdirectory
                  mozdirectory
                  style="position:absolute;opacity:0;left:0;width:100%;height:30px;cursor:pointer;"
                  @change="submintfiles">
                <el-button>
                  <i class="el-icon-more" />
                </el-button>
              </div>
            </el-col>
          </el-row>
        </el-form-item>
        <el-form-item label="">
          <el-checkbox v-model="checked">
            {{ $t('disk.setDefaultPath') }}
          </el-checkbox>
        </el-form-item>
      </el-form>
    </div>
    <div slot="footer">
      <el-button
        id="volume_netDisk--dialog-download-confirm"
        type="primary"
        @click="confirmDownload">
        {{ $t('resource.confirm') }}
      </el-button>
      <el-button
        id="volume_netDisk--dialog-download-cancel"
        class="close-button"
        @click="cancelDownload">
        {{ $t('resource.cancel') }}
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
    dialogDownloadVisible: {
      type: Boolean,
      default: false
    }
  },
  data () {
    return {
      visible: this.dialogDownloadVisible,
      checked: false,
      path: ''
    }
  },
  computed: {

  },
  watch: {
    dialogDownloadVisible (newval) {
      this.visible = newval
    }
  },
  created () {

  },
  mounted () {

  },
  methods: {
    // 取消修改文件名
    cancelDownload () {
      this.$emit('update:dialogDownloadVisible', false)
    },
    confirmDownload () {
      this.$emit('confirmDownload', {path: this.path, checked: this.checked})
    },
    submintfiles (file) {
      this.path = file.target.files[0].path
    }
  }
}
</script>

<style scoped lang="scss">
  .netDisk-icon{
    width:30px;
    height:30px;
    vertical-align: middle;
  }
</style>

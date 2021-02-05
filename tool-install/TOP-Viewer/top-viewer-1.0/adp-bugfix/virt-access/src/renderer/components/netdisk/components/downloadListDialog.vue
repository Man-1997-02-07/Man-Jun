<template>
  <el-dialog
    id="volume_netDisk--dialog-downloadList"
    :title="$t('disk.downloadFiles')"
    :visible.sync="visible"
    :close-on-click-modal="false"
    :show-close="true"
    width="685px"
    @close="closeDialog">
    <div class="wrapper">
      <el-table
        :data="downloadList"
        tooltip-effect="light"
        height="90%">
        <el-table-column
          prop="name"
          :label="$t('disk.fileName')"
          :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <img
              class="netDisk-icon"
              :src="getFileIcon(scope.row.type)">
            <i :class="['netDisk-icon', 'sg-iconfont', 'im-icon-'+getFileIcon(scope.row.type)]" />
            {{ scope.row.name }}
          </template>
        </el-table-column>
        <el-table-column
          prop="size"
          :label="$t('disk.size')" />
        <el-table-column
          prop="percentage"
          align="center"
          :label="$t('disk.downloadProgress')">
          <template slot-scope="scope">
            <span v-if="scope.row.isSuccess">
              <template v-if="scope.row.percentage < 100">
                <el-progress
                  :stroke-width="8"
                  :show-text="false"
                  :percentage="scope.row.percentage" />
              </template>
              <template v-else>{{ $t('disk.downloadSuccess') }}</template>
            </span>
            <span
              v-else
              style="color:#df1130;">
              {{ $t('disk.downloadFailed') }}
            </span>
          </template>
        </el-table-column>
        <el-table-column
          width="180"
          align="center"
          :label="$t('disk.opt')">
          <template slot-scope="scope">
            <span
              v-if="scope.row.isDownloading"
              @click="stopDownload(scope.row)">
              <i class="el-icon-video-pause" />
            </span>
            <span
              v-else
              @click="startDownload(scope.row)">
              <i class="el-icon-video-play" />
            </span>
            <span @click="removeDownload(scope.row)">
              <i class="el-icon-close" />
            </span>
          </template>
        </el-table-column>
      </el-table>
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
    download_fileList: {
      type: Array,
      default: []
    },
    dialogDownloadListVisible: {
      type: Boolean,
      default: false
    },
    getFileIcon: {
      type: Function,
      default: function () {}
    },
    fileTypes: {
      type: Object,
      default: {}
    },
    fileIcon: {
      type: Object,
      default: {}
    }
  },
  data () {
    return {
      visible: this.dialogDownloadListVisible,
      downloadList: []
    }
  },
  computed: {

  },
  watch: {
    dialogDownloadListVisible (newval) {
      this.visible = newval
    }
  },
  created () {

  },
  mounted () {
    this.downloadList = this.download_fileList
  },
  methods: {
    // 取消修改文件名
    closeDialog () {
      this.$emit('update:dialogDownloadListVisible', false)
    },
    stopDownload (row) {},
    startDownload (row) {},
    removeDownload (row) {}
  }
}
</script>

<style scoped lang="scss">
  .wrapper{
    min-height: 361px;
    max-height: 500px;
    padding-bottom: 20px;
  }
  .netDisk-icon{
    width:30px;
    height:30px;
    vertical-align: middle;
  }
  i{
    font-size: 18px;
    &:hover{
      color: #3583E3;
    }
  }
</style>

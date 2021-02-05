<template>
  <el-dialog
    id="volume_netDisk--dialog-move"
    :title="$t('disk.remove')"
    :visible.sync="dialogMoveVisible"
    :show-close="false"
    :close-on-click-modal="false"
    width="685px">
    <div>
      <div style="margin-bottom:10px;">
        <span>
          <span
            id="volume_netDisk--dialog-move--back"
            :style="bucketsPath.length ? 'color:#3583E3' : 'color:#666'"
            @click="backBeforeBucket">{{ $t('disk.back') }}</span>
          <span> | </span>
        </span>
        <span
          id="volume_netDisk--dialog-move--all"
          style="color:#3583E3"
          @click="getAllBuckets">{{ $t('disk.allFiles') }}</span>
        <span
          v-for="(item,index) in bucketsPath"
          :key="index"> > {{ item }}</span>
      </div>
      <el-table
        id="volume_netDisk--dialog-move-table"
        :data="allBuckets"
        tooltip-effect="light"
        height="200px"
        @row-click="checknetworkDiskFiels">
        <el-table-column
          prop="name"
          :label="$t('disk.fileName')"
          :show-overflow-tooltip="true">
          <template slot-scope="scope">
            <img
              class="netDisk-icon"
              src="../../../assets/images/netdisk/wenjianjia.png">
            {{ scope.row.name }}
          </template>
        </el-table-column>
      </el-table>
    </div>
    <div slot="footer">
      <el-button
        id="volume_netDisk--dialog-move--confirm"
        type="primary"
        :disabled="innerLoading || bucketsPath.length === 0 || isNotClick"
        @click="confirmMoveFile">
        {{ $t('resource.confirm') }}
      </el-button>
      <el-button
        id="volume_netDisk--dialog-move--cancel"
        class="close-button"
        @click="cancelMoveFile">
        {{ $t('resource.cancel') }}
      </el-button>
    </div>
    <vs-loading
      :is-show="innerLoading"
      class-name="vs-inner-loading" />
  </el-dialog>
</template>

<script>
import NetDiskOpt from './netDiskOpt.js'
export default {
  components: {

  },
  props: {
    dialogMoveVisible: {
      type: Boolean,
      default: false
    },
    netDiskOpttVisible: {
      type: Boolean,
      default: false
    },
    isShow: {
      type: Boolean,
      default: false
    },
    isNotClick: {
      type: Boolean,
      default: false
    },
    awsS3Client: {
      type: Object
    },
    allBuckets: {
      type: Array,
      default: []
    },
    moveDiaFiles: {
      type: Array,
      default: []
    },
    cur_buckets: {
      type: Array,
      default: []
    },
    batchSelectObj: {
      type: Array,
      default: []
    },
    moveFileObj: {
      type: Object
    },
    moveType: Number,
    curClusterUuid: {
      type: String
    }
  },
  data () {
    return {
      bucketsPath: []
      // s3MoveRequest: null
    }
  },
  mixins: [NetDiskOpt],
  computed: {

  },
  watch: {
    dialogMoveVisible (newval) {
      if (newval) {
        this.getOptDirList()
        this.operateType = 'move'
      }
    }
  },
  created () {

  },
  mounted () {

  },
  methods: {
    // 返回上一级
    backBeforeBucket () {
      if (this.bucketsPath.length) { this.$emit('updateBackBefore', this.bucketsPath) }
    },
    // 全部文件
    getAllBuckets () {
      this.bucketsPath = []
      this.$emit('updateAllBuckets')
    },
    // 查看文件夹
    checknetworkDiskFiels (row, column) {
      const self = this
      if (row.type === 'folder') {
        if (self.bucketsPath.length === 0) {
          self.bucketsPath.push(row.name)
        } else {
          self.bucketsPath.push(row.name)
        }
        if (self.cur_buckets.join('/') === self.bucketsPath.join('/')) {
          self.$emit('bucketPathChange', true)
        } else {
          if (self.cur_buckets.join('/') === self.bucketsPath.slice(0, self.cur_buckets.length).join('/')) {
            if (self.cur_buckets.length > 0) {
              if (self.batchSelectObj) {
                const arr1 = self.batchSelectObj.filter(item => {
                  return item.name === row.name
                })
                self.$emit('bucketPathChange', arr1.length > 0)
              }
            } else {
              self.$emit('bucketPathChange', false)
            }
          } else {
            self.$emit('bucketPathChange', false)
          }
        }
        self.$emit('updateBucketObject', self.bucketsPath, true)
      }
    },
    // 取消移动
    cancelMoveFile () {
      this.$emit('update:dialogMoveVisible', false)
      this.$emit('cancelNetDiskInfo')
      this.bucketsPath = []
      this.sourceDirList = []
      this.sourceFile = []
      this.operateDirList = []
      this.operateFile = []
      this.sourceAllFile = []
      this.operateAllFile = []
      this.tabelSameFiles = []
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

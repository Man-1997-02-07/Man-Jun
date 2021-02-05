<template>
  <div>
    <el-dialog
      :title="`${$t('resource.tip')}`"
      :visible.sync="optNoticeDialog.isShow"
      :close-on-click-modal="false"
      :modal-append-to-body="false"
      width="410px"
      @close="knowAndClose">
      <p>
        {{ optNoticeDialog.info }}
      </p>
      <p>
        <el-radio
          v-model="type"
          label="1">
          {{ time }}秒后执行
        </el-radio>
        <el-radio
          v-model="type"
          label="2">
          {{ $t('resource.cancelExecute') }}
        </el-radio>
      </p>
      <div class="button-label">
        <el-button
          v-if="type==2"
          id="receive_check_opt_confirm"
          type="primary"
          @click="cancelExe">
          {{ $t('resource.confirm') }}
        </el-button>
        <el-button @click="knowAndClose">
          {{ $t('system.Ikown') }}
        </el-button>
      </div>
      <div style="clear:both;" />
    </el-dialog>
  </div>
</template>
<script>
import { mapState } from 'vuex'
import axios from 'axios'
export default {
  computed: {
    ...mapState('Computers', ['cur_StartedVmList'])
  },
  data () {
    return {
      type: '1',
      time: 300,
      timeReduce: '',
      delayTime: ''
    }
  },
  methods: {
    cancelExe () { // 取消执行
      this.setState({
        attr: 'optNoticeDialog',
        val: {
          isShow: false,
          optType: '',
          info: '',
          time: 300,
          exeFlag: false
        }
      })
    },
    knowAndClose () {
      this.setState({
        attr: 'optNoticeDialog',
        val: {
          isShow: false,
          optType: this.optNoticeDialog.optType,
          info: '',
          time: this.time,
          exeFlag: true
        }
      })
    }
  },
  mounted () {
    this.time = this.optNoticeDialog.time
    this.timeReduce = setInterval(() => {
      this.time--
    }, 1000)
  }

}
</script>
<style lang="scss" scoped>
p{
  height: 40px;
}
.button-label{
 margin-bottom: 10px;
    margin-top: 20px;
    float: right;
    .primary{
      background-color: #3583e3;
      color:white;
    }
}
</style>

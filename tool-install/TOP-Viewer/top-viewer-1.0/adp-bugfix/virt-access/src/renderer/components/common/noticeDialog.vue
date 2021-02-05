<template>
  <div
    v-if="isShow"
    class="notice-cont">
    <!-- -->
    <div class="notice-title">
      <i class="sg-iconfont im-icon-gaojing" />
      {{ $t('system.tip') }}<i
        class=" close el-icon-close"
        @click="closeNotice" />
    </div>
    <div class="notice-msg">
      <p>{{ notice_msg }}</p>
      <el-button
        class="close-notice"
        @click="closeNotice">
        {{ $t('system.Ikown') }}
      </el-button>
    </div>
  </div>
</template>
<script>
import { mapState } from 'vuex'
import axios from 'axios'
export default {
  props: ['isShow'],
  computed: {
    ...mapState('Computers', ['cur_StartedVmList'])
  },
  data () {
    return {
      showNotice: false,
      notice_msg: '',
      noticeType: 2,
      iconType: 0// 1表示普通提示 0 表示异常
    }
  },
  methods: {
    closeNotice () {
      this.showNotice = false
      this.setState({
        attr: 'isOpenNoticeDialog',
        val: false
      })
    },
    sendtoConsole (msg) {
      this.cur_StartedVmList.forEach(item => {
        let url = ':' + item.HttpPort + '/top-viewer/notify'
        axios.post(url, {content: msg}).then(res => {
        })
      // 通知虚拟机
      })
    },
    setValue (val) {
      this.notice_msg = val.msg
      this.noticeType = val.type
      this.sendtoConsole(val.msg)
    }
  },
  mounted () {
    this.setValue(this.noticeMsg)
  },
  watch: {
    noticeMsg (val) {
      this.setValue(val)
      // if (val.type !== 2) {
      //   setTimeout(() => {
      //     this.closeNotice()
      //   }, 6000)
      // }
    }
  }

}
</script>
<style lang="scss" scoped>
.notice-cont{
  width:478px;    z-index: 2;
  min-height: 160px;
  background:rgba(255,255,255,1);
  box-shadow:0px 0px 4px 0px rgba(230,227,227,1);
  border-radius:6px;
  border:1px solid rgba(236,238,246,1);
}
.notice-title{
  height: 40px;
  padding: 0px 10px;
  line-height: 40px;
  border: 1px solid #ECEEF6;
  font-size:16px;
  font-weight:bold;
  // color:rgba(50,49,50,1);
  background-color: #3583e3;
  color: white;
  .im-icon-gaojing{
        color: #E6A23C; padding-right:5px ;
  }
  .close{
    font-size: 25px;cursor: pointer;
    position: absolute;
    right: 10px;
    top: 7px;
    color: white;
  }
}
.notice-msg{
  padding: 10px;
  p{
    font-size:14px;
    font-weight:400;
    color:rgba(50,49,50,1);
    line-height:20px;    word-wrap: break-word;
    height: auto;
  }
}
.close-notice{
    position: absolute;
    bottom: 10px;
    right: 10px;
}
</style>

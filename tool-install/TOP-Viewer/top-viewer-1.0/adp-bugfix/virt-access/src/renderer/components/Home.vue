<template>
  <div style="height:100%;">
    <top-label />
    <div class="vm-container">
      <div :class="!expandFlag?'guide-label':'guide-label-expand'">
        <guide v-show="!expandFlag" />
      </div>
      <div
        class="expand"
        :style="expandFlag?'left: 10px;':''">
        <i
          :class="expandFlag?'el-icon-d-arrow-right': 'el-icon-d-arrow-left'"
          @click="expandGuide" />
      </div>
      <div :class="!expandFlag?'cont-label':'cont-label-expand'">
        <router-view v-if="isRouterAlive" />
      </div>
      <el-dialog
        :title="`${$t('resource.tip')}`"
        :visible.sync="isShowAdminAuthDialog"
        :close-on-click-modal="false"
        :modal-append-to-body="false"
        :show-close="false"
        width="410px"
        @close="isShowAdminAuthDialog = false">
        <div style="height:75px;display: flex;padding-top: 15px;">
          <i class="sg-iconfont im-icon-gaojing" />
          <span style="padding-left: 10px;">
            {{ $t('resource.readyUpgrade') }}</span>
        </div>
      </el-dialog>
    </div>
  </div>
</template>
<script>
import { mapState, mapMutations } from 'vuex'
import topLabel from './common/banner/topLabel.vue'
import guide from './common/guide.vue'
import websocketMassage from '@/components/common/js/websocketMessage.js'
export default {
  components: {topLabel, guide},
  mixins: [websocketMassage],
  data () {
    return {
      showScreenOpt: false,
      expandFlag: false,
      isRouterAlive: true,
      wsCount: 0
    }
  },
  computed: {
    ...mapState('Computers', ['cur_StartedVmList']),
    ...mapState('SystemConfig', ['hostId', 'hostOs', 'agentVersion'])
  },
  provide () {
    return {
      reloadRouter: this.reload
    }
  },
  methods: {
    ...mapMutations('SystemConfig', ['setHostInfos']),
    ...mapMutations('Computers', ['changeStartedVmList']),
    reload () { // 切换集群后刷新对应路由
      this.isRouterAlive = false
      setTimeout(() => {
        this.isRouterAlive = true
      })
    },
    expandGuide () {
      if (!this.expandFlag) {
        this.expandFlag = true
      } else {
        this.expandFlag = false
      }
    },
    // 获取websocket连接
    getSocket () {
      // 获得消息事件
      if (typeof (WebSocket) == 'undefined') {
        this.$alert('您使用的程序赞不支持WebSocket', this.$t('resource.tip'), {
          confirmButtonText: this.$t('resource.confirm'),
          type: 'info',
          callback: action => {}
        })
        this.$router.push('/Login')
      } else {
        this.createSocket()
        // 打开事件，先获取cpu等信息，将他们发给服务器，建立连接
        // 如果连接建立失败，按理是不能使用的
        localStorage.setItem('noErrorTipFlag', true)
        this.$Api.system.hostUsedMsg().then(res => {
          localStorage.removeItem('noErrorTipFlag')
          if (res.Status.code == 0) {
            this.socket.addEventListener('open', function () {
            })
            let Disk = []
            if (res.Resource.Disk) {
              res.Resource.Disk.forEach(item => {
                Disk.push({
                  name: item.Name,
                  total: item.Total,
                  used: item.Used,
                  free: item.Free,
                  used_percent: item.UsedPercent
                })
              })
            }
            let data = {
              cluster_uuid: this.cluster_uuid,
              id: this.hostId,
              description: 'xxx',
              os_version: this.hostOs,
              agent_version: this.agentVersion,
              api_key: this.ApiKey,
              agent_resource: {
                time: parseInt((new Date()).getTime() / 1000),
                resource: {
                  cpu_number: res.Resource.CpuNum,
                  cpu_used_percent: res.Resource.CpuUsedPercent,
                  memory_total: res.Resource.MemTotal,
                  memory_used: res.Resource.MemUsed,
                  memory_free: res.Resource.MemFree,
                  memory_used_percent: res.Resource.MemUsedPercent,
                  disks: Disk
                }
              }
            }
            let params = {'scode': 0, 'desc': '', 'message': '', 'message_cn': '', 'stack': null, 'data': data, 'type': 0}
            var TT = 0
            this.socket.onopen = () => {
              TT = 1
              this.socket.send(JSON.stringify(params))
              this.socketMessage()
            }
            if (TT == 0) {
              this.socket.send(JSON.stringify(params))
              this.socketMessage()
            }
          }
        }).catch(() => {
          // 与服务器连接建立失败，请重试或确认vdiagent已启动
          this.$message({
            message: this.$t('login.continueErrorTip'),
            type: 'warning',
            duration: 6000
          })
          this.$router.push('/Login')
          localStorage.removeItem('noErrorTipFlag')
        })
        this.socket.onerror = () => {
        }
        this.socket.onclose = (e) => {
          console.log('e：', e)
          this.$nprogress.start()
          this.setState({
            attr: 'isOpenNoticeDialog',
            val: false
          })
          this.$Api.computers.closeAllVms({}).then(res => { // 一键杀死topview拉起的remote-view
          })
          // this.$Api.computers.clearVms({agent_client_id: this.hostId, cluster_uuid: this.cluster_uuid}).then(res => { // 清空普通用户桌面池里分配的虚拟机
          //   console.log('clearVms', res)
          // })
          this.changeStartedVmList({type: 'close'}) // 制空当前拉起的虚拟机
          setTimeout(() => {
            this.$nprogress.done()
            sessionStorage.clear()
            this.updateMsg()
            this.$router.push('/Login')
          })
        }
        // 关闭ws后刷新列表
        // if (!this.handlerCloseWs && !this.isTimeOut && !this.isKickedOut.value && this.wsCount < 10000 && this.$route.path !== '/login') {
        //   this.wsCount++
        //   // this.updateUrl();
        //   // this.getSocket();
        // this.tipstimer = setTimeout(function () {
        //   clearInterval(this.tipstimer)
        //   window.location.reload()
        // }, 3500)
        // }
        // }
      }
    }
  },
  mounted () {
    this.getSocket()
  }
}
</script>
<style lang="scss" scoped>
.vm-container{
display: flex;height: 100%;
}
.guide-label{
  width:220px;
}
.guide-label-expand{
  width:0px;
}
.cont-label{
  width: calc(100% - 230px);
}
.cont-label-expand{
  width: calc(100% - 10px);
}
.expand{
    font-size: 20px;
    position: absolute;
    top: 50%;
    height: 40px;
    left: 220px;z-index: 2;
    cursor: pointer;
}
  .im-icon-gaojing{
        color: #E6A23C; padding-right:5px ;
  }
</style>

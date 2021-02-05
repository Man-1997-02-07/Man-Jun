<template>
  <div id="app">
    <div
      v-if="!setting.hide_navigation_bar&&isWin"
      class="desktop-opt-cont"
      @mouseleave="showScreenOpt=false"
      @mouseenter="showScreenOpt=true">
      <div
        v-show="showScreenOpt"
        class="desktop-size-opt">
        <el-tooltip
          effect="light"
          :content="$t('system.fullscreen')">
          <i
            class="sg-iconfont im-icon-quanping3"
            @click="getFullScreen" />
        </el-tooltip>
        <el-tooltip
          effect="light"
          :content="$t('system.maxsize')">
          <i
            class="sg-iconfont im-icon-zuidahua"
            @click="getMaximize" />
        </el-tooltip>
        <el-tooltip
          effect="light"
          :content="$t('system.regularsize')">
          <i
            class="sg-iconfont im-icon-changguihua"
            @click="getNormalSize" />
        </el-tooltip>
        <el-tooltip
          effect="light"
          :content="$t('system.close')">
          <i
            class="sg-iconfont im-icon-guanbi"
            @click="closeBox" />
        </el-tooltip>
        <el-tooltip
          effect="light"
          :content="$t('system.openDevtool')">
          <i
            style="margin-right:20px;font-size:30px;font-weight: 600;"
            class="sg-iconfont im-icon-xiangqing"
            @click="openCosole" />
        </el-tooltip>
      </div>
    </div>
    <div class="router-cont">
      <router-view />
      <noticeDialog
        v-if="isOpenNoticeDialog"
        :isShow="isOpenNoticeDialog"
        class="notice-dialog" />
      <optNoticeDialog
        v-if="optNoticeDialog.isShow"
        id="receive_notice_opt_dialog" />
    </div>
  </div>
</template>
<script>
import { ipcRenderer } from 'electron'
import noticeDialog from '@/components/common/noticeDialog'
import optNoticeDialog from '@/components/common/optNoticeDialog'
var os = require('os')
export default {
  name: 'VirtAccess',
  components: {noticeDialog, optNoticeDialog},
  data () {
    return {
      showScreenOpt: false,
      isWin: true
    }
  },
  methods: {
    getFullScreen () {
      ipcRenderer.send('fullScreen')
    },
    getMaximize () {
      ipcRenderer.send('maximize')
    },
    closeBox () {
      setTimeout(() => {
        ipcRenderer.send('closed')
      }, 100)
      this.closeSocket()
    },
    getNormalSize () {
      ipcRenderer.send('normal')
    },
    openCosole () {
      ipcRenderer.send('openConsole')
    }
  },
  created () {
    if (!os.platform().includes('win')) {
      this.isWin = false
    }
  },
  mounted () {
    ipcRenderer.on('close-screen', (event, arg) => {
      setTimeout(() => {
        ipcRenderer.send('cleanok')
      }, 500)
      this.closeSocket()
    })
    ipcRenderer.on('reply', (event, arg) => {
    })
  },
  destroyed () {

  }
}
</script>

<style lang="scss">
#app{
  position: absolute;
  top: 0px;left: -0.5px;
  bottom: 0px;
  width: 100%;
  overflow: hidden;
  background: white;
}
.el-loading-mask{
    opacity: 0.45 !important;
}
.notice-dialog{
  position: absolute;
    right: 20px;bottom: 20px;
}
.router-cont{
  height: 100%;
}
.desktop-opt-cont{
    position: absolute;
    margin: auto;    z-index: 2000;
    left: 40%;width: 288px;height:50px;
    display: inline-block;

}
.desktop-size-opt{
    display: inline-block;
    background-color: white;
    border: 1px solid #666;padding: 10px 0;
    /* text-align: center; */
    i{
      color: #323132;
      font-size: 30px;
      cursor: pointer;
      margin-left: 20px;
    }
}
  /* CSS */
</style>

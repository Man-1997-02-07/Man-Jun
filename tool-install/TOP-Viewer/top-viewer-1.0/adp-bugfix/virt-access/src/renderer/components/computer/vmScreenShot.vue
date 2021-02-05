<template>
  <div style="height:80%;">
    <!-- 虚拟机缩略图 -->
    <div
      class="vm-screen-shot">
      <div
        v-if="showimage === 'success'"
        v-loading="loading">
        <!-- v-for="(item,index) in images"
        id="img-src"
        ref="imgPhone"
        :key="index"
        class="img-style" -->
        <img
          id="vm_img"
          alt="图片加载失败"
          :src="images">
      </div>
      <div v-if="showimage === 'error'">
        <img
          class="img-style"
          src="@/assets/images/icons/error-screenshot.png"
          alt="">
      </div>
      <div v-if="showimage === 'black'">
        <img
          class="img-style"
          src="@/assets/images/icons/black-screenshot.png"
          alt="">
      </div>
      <div v-if="showimage == '25044'">
        <img
          class="img-style"
          src="@/assets/images/icons/error-screenshot-25044.png"
          alt="">
      </div>
      <div v-if="showimage == '25045'">
        <img
          class="img-style"
          src="@/assets/images/icons/error-screenshot-25045.png"
          alt="">
      </div>
    </div>

    <div class="bottom-button">
      <el-button
        type="primary"
        @click="getConsole(cur_guide_item)">
        {{ $t('vm.connection') }}
      </el-button>
    </div>
  </div>
</template>
<script>
import { mapState, mapMutations } from 'vuex'
import Basic from '@/test/basic.js'
import { decryptAESPassword } from '@/components/common/js/aesPassword'
// import '@/components/common/js/webconfig'
const exec = require('child_process').exec
var os = require('os')
export default {
  data () {
    return {
      setInterSnap: null,
      showimage: '',
      images: '',
      // cur_vm_img: this.cur_guide_item,

      vm_info: {
        name: '',
        zone_name: '',
        object_group_name: '',
        cpu: '', // cpu
        cpu_max: '',
        memery_max: '',
        memery_max_unit: 'G',
        memery: '',
        memery_unit: 'G',
        start_set: '',
        agent: ''
      }
    }
  },
  methods: {
    ...mapMutations('Computers', ['changeStartedVmList']),
    getComputeScreenShot (computerAttr) {
      if (computerAttr.state === 'shutoff' || computerAttr.action == 'save') {
        // 关机机清掉
        clearInterval(this.setInterSnap)
        this.setInterSnap = null
        this.showimage = 'black'
        return
      }
      this.getScreenShotCommon(computerAttr)
    },
    getScreenShotCommon (computerAttr) {
      // 判断当前租户的quote 有没有vm_screenshot_space  vm_screenshot_space  > 0 则继续调用
      this.$Api.tenant.tenantQuota({ cluster_uuid: computerAttr.cluster_uuid, tenant: computerAttr.tenant }).then(res => {
        if (res.scode === 0) {
          const data = res.data.quotas[0]
          if (data && data.quota && data.quota.vm_screenshot_space > 0) {
            // 调用定时器之前 先清除
            clearInterval(this.setInterSnap)
            this.setInterSnap = null
            // 其他情况正常获取
            // 第一次调用
            this.getComputeScreenShotMethod(computerAttr)
            // 后面10秒一次
            this.setInterSnap = setInterval(() => {
              this.getComputeScreenShotMethod(computerAttr)
            }, 10000)
          } else {
            // vm_screenshot_space 没开启截图功能
            this.showimage = 'error'
          }
        } else {
          // 报错 黑屏
          this.showimage = 'black'
        }
      })
    },
    // 定时器任务
    getComputeScreenShotMethod (computerAttr) {
      const params = {
        cluster_uuid: computerAttr.cluster_uuid,
        tenant: computerAttr.tenant,
        compute_name: computerAttr.name,
        compute_uuid: computerAttr.uuid
      }
      this.loading = true
      this.$Api.computers.computeScreenShot(params).then((res) => {
        this.loading = false
        this.images = ''
        // 正常拿到
        if (res.scode === 0) {
          const data = res.data
          this.showimage = 'success'
          this.images = data.screenshot_url + '?t=' + Math.random()
          // this.images.push({
          //   src: data.screenshot_url + '?t=' + Math.random()
          // })
        } else {
          clearInterval(this.setInterSnap)
          this.setInterSnap = null
          delete this.setInterSnap
          // 目前卷损坏的情况不能完全避免，考虑在卷不可用的情况下
          // 前端根据后端返回的错误来决定在显示截图处显示对应的错误信息图片即可，不在界面弹出截图错误信息
          if (res.scode == 25044) {
            this.showimage = '25044'
          } else if (res.scode == 25045) {
            this.showimage = '25045'
          } else if (res.scode == 22184) {
            this.showimage = 'black'
          } else {
            this.showimage = 'black'
          }
        }
      }).catch((err) => {
        return err
      })
    },
    // 桌面控制 打开VM
    getConsole (val) {
      if (val.state != 'running' && val.state != 'suspend') {
        this.$message({
          message: `${this.$t('vm.startFirstly')}`,
          type: 'warning',
          duration: 3000
        })
        return false
      }
      const param = {
        compute_uuid: val.uuid,
        tenant: val.tenant,
        cluster_uuid: val.cluster_uuid ? val.cluster_uuid : this.cluster_uuid
      }
      this.$Api.computers.vmInfo(param).then(async (res) => {
        if (res.scode == 0) {
          const data = res.data
          if (data.machine_manager_ip) {
            const spicename = encodeURIComponent(data.name)
            data.spice_password && (data.spice_password = decryptAESPassword(data.spice_password, this.aesConfig.aesIv, this.aesConfig.aesKey))

            let waterSetting = ''
            let otherSetting = ''
            if (this.setting.uuid) {
              otherSetting = ' ' + ((this.setting.allow_usb_redirect === false) ? '--disable-usb-usbredir --spice-usbredir-auto-redirect-filter="-1,-1,-1,-1,0" ' : '') +
                (((this.setting.drag_drop_files === false && !this.securitySetting) || this.securitySetting.drag_file === false) ? '--disable-drag-file ' : '') +
                (((this.setting.allow_pc_clipboard === false && !this.securitySetting) || this.securitySetting.share_clipboard === false) ? '--disable-auto-clipboard ' : '')
              if (this.setting.enable_screen_watermark && this.setting.watermark_content) { // 水印是开启状态，并且水印有内容
                waterSetting = '#' + this.setting.watermark_content + ',' + this.setting.watermark_move_time + ',' + (this.setting.lean ? this.setting.lean : 'normal') + ',' + this.setting.font_size + ',' + this.setting.font_thickness + ',' + this.setting.content_color + ',' + this.setting.show_style + ',' + (this.setting.underline_type ? this.setting.underline_type : 'none') + ',' + (this.setting.background_color ? this.setting.background_color !== 'transparent' ? (this.setting.background_color + ',0') : 'pink,1.0' : 'none') + ',' + this.setting.watermark_coordinate_x + ',' + this.setting.watermark_coordinate_y
              }
            }
            let url

            if (data.spice_port_mapping) {
              // 开启了spice映射
              const res = await this.$Api.computers.computeVDCConsole({
                cluster_uuid: val.cluster_uuid,
                tenant: val.tenant,
                uuid: val.uuid
              })
              if (res.scode !== 0) {
                this.$message({
                  message: res.message_cn,
                  type: 'error',
                  duration: 3000
                })
                return false
              }
              if (this.testIsIpv6Type(res.data.vdc_console)) {
                url = `"spice://[${res.data.vdc_console}]` + '?' + spicename + ',' + data.spice_password + waterSetting + '"' + otherSetting
              } else {
                url = '"spice://' + `${res.data.vdc_console}` + '?' + spicename + ',' + data.spice_password + waterSetting + '"' + otherSetting
              }
            } else {
              if (this.testIsIpv6Type(data.machine_manager_ip)) {
                url = `"spice://[${data.machine_manager_ip}]` + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + waterSetting + '"' + otherSetting
              } else {
                url = '"spice://' + data.machine_manager_ip + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + waterSetting + '"' + otherSetting
              }
            }

            // 打开rometeviewer
            this.getFreePort().then(port => {
              this.getCMD(val, url, port)
            })
          }
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },

    getCMD (val, cmd, port) {
      // 启动指定的虚拟机的时，通过websocket向服务器发送一个选定虚拟机的信息，
      let compute_info = []
      if (sessionStorage.getItem('compute_info')) {
        compute_info = JSON.parse(sessionStorage.getItem('compute_info'))
      }
      compute_info.push({'cluster_uuid': this.cluster_uuid,
        'tenant': val.tenant,
        'HttpPort': port,
        'compute_uuid': val.uuid,
        'compute_name': val.name })
      sessionStorage.setItem('compute_info', JSON.stringify(compute_info))
      const socketParams = {
        'type': 2,
        'data': {
          'time': Date.parse(new Date()) / 1000,
          'compute_info': compute_info
        }
      }
      let cur_vm = {
        HttpPort: port,
        vmId: val.uuid
      }
      // 用来存当前使用的虚拟机，便于对他进行设置修改
      this.changeStartedVmList({data: cur_vm, type: 'set'})
      this.$message({
        message: this.$t('vm.openTip'),
        type: 'success',
        duration: 3000
      })
      this.socket.send(JSON.stringify(socketParams))
      let path = '"C:\\Program Files\\TopViewer1.0\\bin\\top-viewer.exe" '
      if (!os.platform().includes('win')) {
        path = '/usr/local/bin/top-viewer '
      }
      let cmdline = path + cmd + ' -o ' + port
      exec(cmdline, (error, stdout, stderr) => {
        if (error && this.cur_hostIp) {
          this.$message({
            message: this.$t('vm.startFailed'),
            type: 'error',
            duration: 1500
          })
        }
        var allLog = stderr.replaceAll('\r|\n', '').trim()
        var portStartLog = allLog.substr((allLog.search('HTTP_SERVRR_PORT = ') + 19))
        var cur_port = Number(portStartLog.substr(0, portStartLog.search('top')))
        // 获取关闭的端口号后
        let compute_info = JSON.parse(sessionStorage.getItem('compute_info'))
        let closed_vm_info = compute_info ? compute_info.filter(item => item.HttpPort == cur_port) : [] // 获取到关闭的虚拟机
        let cur_vms_info = compute_info ? compute_info.filter(item => item.HttpPort !== cur_port) : []
        sessionStorage.setItem('compute_info', JSON.stringify(cur_vms_info))
        let closed_vm = closed_vm_info.length > 0 ? closed_vm_info[0] : ''
        this.changeStartedVmList({data: closed_vm.compute_uuid, type: 'delete'})

        const closeVm = {
          'type': 3,
          'data': {
            'time': Date.parse(new Date()) / 1000,
            'cluster_uuid': this.cluster_uuid,
            'tenant': closed_vm.tenant,
            'compute_uuid': closed_vm.compute_uuid,
            'compute_name': closed_vm.compute_name
          }
        }
        this.socket.send(JSON.stringify(closeVm))
        if (this.setting.shutdown_integration && os.platform().includes('win')) { // 需要同步关闭终端
          this.closeSocket()
          setTimeout(() => {
            ipcRenderer.send('cleanok')
          }, 500)
        }
      })
    },
    // 判断是否是Ipv6类型
    testIsIpv6Type (value) {
      return /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/.test(value)
    }
  },
  watch: {
    cur_guide_item (val) {
      this.showimage = ''
      this.getComputeScreenShot(val)
    }
    // '$route' (val) {
    //   this.getComputeScreenShot(this.$route.query)
    // }
  },
  computed: {
    ...mapState('SystemConfig', ['aesConfig'])
  },
  mounted () {
    this.showimage = ''
    this.getComputeScreenShot(this.cur_guide_item)
  },
  destroyed () {
    // 组件死亡清除
    clearInterval(this.setInterSnap)
    this.setInterSnap = null
    delete this.setInterSnap
    // this.setState({
    //   attr: 'cur_guide_item',
    //   val: {}
    // })
  }
  // activated () {
  // }
}
</script>
<style lang="scss" scoped>
.vm-screen-shot{
  height:100%;
  margin-top: 20px;
  margin-left: 20px;
  div{
    height:100%;
    text-align: center;
  }
  img{
    height:100%;max-width: 100%;
    max-height: 100%;
  }
}
.bottom-button{
    position: fixed;
    bottom: 20px;
    right: 20px;
}
.el-button{
	min-width: 72px;
	height: 32px;
	border-radius: 0px;
}
</style>

<template>
  <div
    class="desktop-cont">
    <div
      v-if="!showBlockPic"
      class="vm-opts-label">
      <el-button
        v-if="!isadmin"
        @click="toAddTicket">
        {{ $t('ticket.addTicket') }}
      </el-button>
      <el-checkbox
        v-model="checkAll"
        class="sg-iconfont"
        :indeterminate="isIndeterminate"
        @change="selectAll">
        {{ $t('resource.checkAll') }}
      </el-checkbox>
      <span
        @click="batchVmOpt('bath_start')"><i class="sg-iconfont im-icon-qidong" />{{ $t('cloud.start') }}</span>
      <span
        @click="batchVmOpt('bath_reboot')"><i class="sg-iconfont im-icon-zhongqi1" />{{ $t('cloud.reboot') }}</span>
      <span
        @click="batchVmOpt('bath_shutoff')"><i class="sg-iconfont im-icon-guanji" />{{ $t('cloud.close') }}</span>
    </div>
    <div style="clear:both;" />
    <div class="vm-list">
      <vx-scroll
        :init="true"
        style="height:80%;">
        <el-checkbox-group
          v-model="checkedVms"
          class="vm-checkBox"
          @change="selectVms">
          <div
            v-if="showBlockPic==true"
            class="block-desktop">
            <img src="@/assets/images/icons/block_desktop.png">
            <p>{{ $t('resource.noData') }}</p>
          </div>
          <div v-else>
            <div
              v-for="(item,index) in computerList"
              :key="index"
              class="vm-cont">
              <div
                :class="(cur_vm.uuid==item.uuid?true:false||checkedVms.some(el=>el.uuid==item.uuid))?'vm-pic-checked vm-pic':'vm-pic'"
                @mouseover="showVmOpt(item)"
                @mouseleave="leaveVm">
                <el-checkbox
                  v-show="(cur_vm.uuid==item.uuid?true:false||checkedVms.some(el=>el.uuid==item.uuid))&&item.action=='noaction'"
                  :key="index"
                  class="select-vm"
                  :label="item">
                  .
                </el-checkbox>
                <div
                  v-show="cur_vm.uuid==item.uuid?true:false||checkedVms.some(el=>el.uuid==item.uuid)"
                  class="vm-opts">
                  <!-- 休眠状态需要唤醒操作 -->
                  <el-tooltip
                    v-if="!(item.state !== 'pmsuspend' && (item.action !== 'save' || item.action == 'restoring') || item.action == 'importSuspend')"
                    placement="top"
                    effect="light"
                    :content="$t('cloud.awaken')">
                    <i
                      class="sg-iconfont im-icon-huanxing"
                      @click="computerOpt('restore',item)" />
                  </el-tooltip>
                  <!-- 挂起状态需要恢复操作 -->
                  <el-tooltip
                    v-if="!(!['paused'].includes(item.state) || ['pending','restoring','importSuspend'].includes(item.action))"
                    placement="top"
                    effect="light"
                    :content="$t('cloud.recovery')">
                    <i
                      class="sg-iconfont im-icon-huifu vm-opt-border"
                      @click="computerOpt('resume',item)" />
                  </el-tooltip>
                  <!-- 开机 -->
                  <el-tooltip
                    v-if="!(item.state == 'paused'||item.state == 'pmsuspend' || ['migrating','cloning','save','restoring','importSuspend'].includes(item.action))"
                    placement="top"
                    effect="light"
                    :content="$t('cloud.start')">
                    <i
                      :style="item.state!=='running'?'cursor:pointer;':'cursor:no-drop;'"
                      class="sg-iconfont im-icon-qidong vm-opt-border"
                      @click="item.state!=='running'?computerOpt('start',item):''" />
                  </el-tooltip>
                  <!-- 重启 -->
                  <el-tooltip
                    placement="top"
                    effect="light"
                    :content="$t('cloud.reboot')">
                    <i
                      :style="!(!['running'].includes(item.state) || ['migrating','cloning','save','restoring','importSuspend'].includes(item.action))?'cursor:pointer;':'cursor:no-drop;'"
                      class="sg-iconfont im-icon-zhongqi1 vm-opt-border"
                      @click="!(!['running'].includes(item.state) || ['migrating','cloning','save','restoring','importSuspend'].includes(item.action))?computerOpt('reboot',item):''" />
                  </el-tooltip>
                  <!-- 关机操作 -->
                  <el-tooltip
                    placement="top"
                    effect="light"
                    :content="$t('cloud.close')">
                    <i
                      :style="!(!['running'].includes(item.state) || ['cloning','save','importSuspend'].includes(item.action))?'cursor:pointer;':'cursor:no-drop;'"
                      class="sg-iconfont im-icon-guanji vm-opt-border"
                      @click="!(!['running'].includes(item.state) || ['cloning','save','importSuspend'].includes(item.action))?computerOpt('shutoff',item):''" />
                  </el-tooltip>
                  <el-tooltip
                    placement="top"
                    effect="light"
                    :content="$t('cloud.openconsole')">
                    <i
                      class="sg-iconfont im-icon-bangding"
                      @click="checkConsoleState(item)" />
                  </el-tooltip>
                </div>
                <!-- 图片上的状态 --->
                <span
                  class="state-content"
                  v-html="$t('cloud.vm_state')+getVmState(item.state)+' | '+$t('cloud.act')+getVmAction(item.action)" />
                <div
                  v-if="item.action=='error'||item.action=='waiting'||item.action=='cloning'"
                  class="vm-error">
                  <div
                    v-if="item.action=='error'"
                    class="vm-error-cont">
                    <span><i class="el-icon-circle-close" />{{ $t('vm.vmAddErrorTip') }}</span>
                    <span v-show="language=='zh'">错误信息：{{ item.clone_error_reason_cn }}</span>
                    <span v-show="language=='en'">Error message：{{ item.clone_error_reason }}</span>
                  </div>
                  <div
                    v-else
                    class="vm-cloning-cont">
                    <span v-show="item.action=='cloning'"><i class="el-icon-loading" />  {{ $t('vm.cloning') }}</span>
                    <span v-show="item.action=='waiting'"><i class="el-icon-loading" />  {{ $t('vm.waiting') }}</span>
                  </div>
                </div>
                <!-- <el-tooltip
                  placement="top"
                  effect="light"
                  :content="'状态：'+item.state"> -->
                <div
                  v-else-if="item.os.includes('Windows')"
                  @dblclick="checkConsoleState(item)">
                  <li v-if="item.os.includes('2003')">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/win2003.png">
                    <img
                      v-else
                      src="@/assets/images/vm/win2003-shutoff.png">
                  </li>
                  <li v-if="item.os.includes('2008')">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/win2008.png">
                    <img
                      v-else
                      src="@/assets/images/vm/win2008-shutoff.png">
                  </li>
                  <li v-if="item.os.includes('XP')">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/winxp.png">
                    <img
                      v-else
                      src="@/assets/images/vm/winxp-shutoff.png">
                  </li>
                  <li v-if="item.os.includes('Windows 7')">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/win7.png">
                    <img
                      v-else
                      src="@/assets/images/vm/win7-shutoff.png">
                  </li>
                  <li v-if="item.os.includes('Windows 8')">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/win8.png">
                    <img
                      v-else
                      src="@/assets/images/vm/win8-shutoff.png">
                  </li>
                  <li v-if="item.os.includes('Windows 10')">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/win10.png">
                    <img
                      v-else
                      src="@/assets/images/vm/win10-shutoff.png">
                  </li>
                </div>
                <!-- ="item.os.includes('Ubuntu')||item.os.includes('CentOS')||item.os.includes('Linux')||item.os.includes('Debian')" -->
                <li
                  v-else
                  @dblclick="checkConsoleState(item)">
                  <img
                    v-if="item.state=='running'"
                    src="@/assets/images/vm/linux.png">
                  <img
                    v-else
                    src="@/assets/images/vm/linux-shutoff.png">
                </li>
                <li v-if="['Windows Server 2019 x64','Windows Server 2019','Windows Server 2016','Windows Server 2012','Windows Vista x64','Windows Vista x64 Edition'].includes(item.os)">
                  <img
                    v-if="item.state=='running'"
                    src="@/assets/images/vm/win.png">
                  <img
                    v-else
                    src="@/assets/images/vm/win-shutoff.png">
                </li>
                <!-- </el-tooltip> -->
              </div>
              <el-tooltip
                effect="light"
                :content="item.name">
                <span v-if="item.name.replace(/[\u0391-\uFFE5]/g,'aa').length<20">
                  {{ item.name }}
                </span>
                <!-- <p v-if="item.name.replace(/[\u0391-\uFFE5]/g,'aa').length>15">{{ item.name.substr(0,8) }}...</p> -->
                <span v-else>
                  {{ item.name.substr(0,20) }}...
                </span>
              </el-tooltip>
            </div>
          </div>
        </el-checkbox-group>
      </vx-scroll>
    </div>
    <div>
      <el-button
        class="bottom-button"
        type="primary"
        @click="showSettingDialog">
        {{ $t('resource.setting') }}
      </el-button>
    </div>
    <add-ticket-dialog
      v-if="showAddTicketDialog"
      :computerList="computerList"
      :showAddTicketDialog="showAddTicketDialog"
      @closeAddTicketDialog="closeAddTicketDialog" />
    <system-setting-dialog
      v-if="showSetting"
      :show="showSetting"
      @closeSettingDialog="closeSettingDialog" />

    <vm-opt-dialog
      v-if="isShowVmOptDialog"
      :vms_info="vms_info"
      :opt_type="opt_type"
      :vm_opt_params="vm_opt_params"
      :tipContent="tipContent"
      :isShowVmOptDialog="isShowVmOptDialog"
      @closeOptDialog="closeOptDialog" />
    <!-- 选择控制台类型  -->、
    <vm-console-type-dialog
      v-if="isShowVmConsoleTypeDialog"
      :type_dialog_val="type_dialog_val"
      :isShowVmConsoleTypeDialog="isShowVmConsoleTypeDialog"
      @getConsoleType="getConsoleType" />
  </div>
</template>

<script>
import { ipcRenderer } from 'electron'
import { mapState, mapMutations } from 'vuex'
import Basic from '@/test/basic.js'
import systemSettingDialog from '../common/systemSettingDialog'
import vmOptDialog from '../common/vmOptDialog'
import addTicketDialog from './addTicketDialog'
import { decryptAESPassword } from '@/components/common/js/aesPassword'
import vmConsoleTypeDialog from '../common/vmConsoleTypeDialog'
import ElectronStore from 'electron-store'
const exec = require('child_process').exec
var os = require('os')
var electronStore = new ElectronStore() // 存储数据
export default {
  mixins: [Basic],
  components: {systemSettingDialog, addTicketDialog, vmOptDialog, vmConsoleTypeDialog},
  data () {
    return {
      isShowVmConsoleTypeDialog: false,
      type_dialog_val: '',
      desktopValue: '',
      filter_fuzzy: '',
      computerList: [],
      cur_computer: {
        name: ''
      },
      cur_vm: '',
      showSetting: false,
      checkedVms: [],
      checkAll: false,
      isIndeterminate: false,
      showBlockPic: false,
      showAddTicketDialog: false,
      isShowVmOptDialog: false,
      tipContent: '',
      opt_type: '',
      vms_info: '',
      vm_opt_params: '',
      VSFlag: {
        UK: '未知',
        vm: {
          // 虚拟机动作--0: noaction, 1: cloning, 2: save, 3: migrating, 4: pending, 5: importing 6.unknown 7.restoring
          unknown: '未知',
          noaction: '无动作',
          cloning: '克隆中',
          migrating: '迁移中',
          importing: '导入中',
          importSuspend: '导入中断',
          save: '休眠',
          pending: '待定',
          restoring: '备份恢复中',
          // 新加 -attr
          importfail: '导入失败',
          exporting: '导出中',
          exportfail: '导出失败'
        }
      }
    }
  },
  methods: {
    ...mapMutations('Computers', ['changeStartedVmList']),
    showVmOpt (vm) {
      this.cur_vm = vm
    },
    leaveVm () {
      this.cur_vm = ''
    },
    // 全选
    selectAll (value) {
      this.checkedVms = value ? this.computerList : []
      this.isIndeterminate = false
    },
    // 选择虚拟机
    selectVms (value) {
      let checkedCount = value.length
      this.checkAll = checkedCount === this.computerList.length
      this.isIndeterminate = checkedCount > 0 && checkedCount < this.computerList.length
    },
    closeSettingDialog (val) {
      this.showSetting = false
    },
    showSettingDialog () {
      this.showSetting = true
    },
    toAddTicket () { // 创建工单
      this.showAddTicketDialog = true
    },
    closeAddTicketDialog () {
      this.showAddTicketDialog = false
    },
    getComputerList (val) {
      // if (val) {
      //   // 其他地方调用了，需要保持在虚拟机相关操作页面
      //   this.cur_computer = val
      // } else {
      //   this.cur_computer = ''
      // }
      this.showBlockPic = false
      const param = {
        tenant: this.cur_tenant,
        page_num: 0,
        page_size: 0,
        filter_fuzzy: this.filter_fuzzy,
        filter_host_type: 'GUS,GVPROXY',
        // filter_user_uuid: this.useruuid,
        cluster_uuid: this.cluster_uuid
      }
      if (this.isuser) {
        param.filter_user_uuid = sessionStorage.getItem('useruuid')
      }
      this.setState({
        attr: 'onLoading',
        val: true
      })
      this.$Api.computers.vmList(param).then(res => {
        this.setState({
          attr: 'onLoading',
          val: false
        })
        if (res.scode == 0) {
          this.computerList = res.data.domains
          this.computerList.forEach(item => {
            // 动作 0: noaction, 1: cloning, 2: save, 3: migrating, 4: pending, 5: importing 6.unknown(未知) 7.importSuspend
            item.action = item.action == 'importing' && item.attr && item.attr.ImportSuspend ? 'importSuspend' : item.action
            // item.flagtxt = this.language == 'zh' ? this.VSFlag.vm[item.action] : item.action
          })
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
        if (this.computerList.length == 0) {
          this.showBlockPic = true
        }
      })
    },
    getComputerOpt (val) {
      this.cur_computer = val
    },
    checkConsoleState (val) { // 打开控制台前，检查状态
      if (val.state == 'shutoff' || val.action == 'save' || val.state == 'paused') { // 如果是关机 就开启;如果休眠 就唤醒
        this.$nprogress.start()
        let Api = val.state == 'shutoff' ? this.$Api.computers.vmStart : val.action == 'save' ? this.$Api.computers.vmRestore : this.$Api.computers.vmResume
        let params = {
          cloud_init: true,
          cluster_uuid: val.cluster_uuid,
          compute_name: val.name,
          compute_uuid: val.uuid,
          tenant: val.tenant
        }
        Api(params).then(res => {
          this.$nprogress.done()
          if (res.scode == 0) { // 开机成功，调用虚拟机
            this.getComputerList()
            this.getConsole(val)
          } else {
            this.$message({
              message: res.message_cn,
              type: 'error',
              duration: 3000
            })
          }
        })
      } else {
        this.getConsole(val)
      }
    },
    // 桌面控制 打开VM
    async getConsole (val) {
      // if (val.state != 'running' && val.state != 'suspend') {
      //   this.$message({
      //     message: `${this.$t('vm.startFirstly')}`,
      //     type: 'warning',
      //     duration: 3000
      //   })
      //   return false
      // }
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
            // let ttt = encodeURIComponent(' -d -f')

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
              // 给弹框 判断是使用内网还是外网
              // 先判断是否有设置
              this.type_dialog_val = {
                val: val,
                data: data,
                spicename: spicename,
                settingInfo: settingInfo,
                otherSetting: otherSetting}
              let consoleSetType = electronStore.get('consoleType') || localStorage.getItem('consoleType')
              if (consoleSetType) {
                this.getConsoleType({type: consoleSetType, data: this.type_dialog_val})
              } else {
                this.isShowVmConsoleTypeDialog = true
              }
            } else {
              if (this.testIsIpv6Type(data.machine_manager_ip)) {
                url = `"spice://[${data.machine_manager_ip}]` + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + waterSetting + '"' + otherSetting
              } else {
                url = '"spice://' + data.machine_manager_ip + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + waterSetting + '"' + otherSetting
              }
              // 打开rometeviewer
              this.getFreePort().then(port => {
                this.getCMD(val, url, port)
              })
            }
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
        duration: 1500
      })
      this.socket.send(JSON.stringify(socketParams))
      let path = '"C:\\Program Files\\TopViewer1.0\\bin\\top-viewer.exe" '
      if (!os.platform().includes('win')) {
        path = '/usr/local/bin/top-viewer '
      }
      let cmdline = path + cmd + ' -o ' + port
      console.log('cmdline', cmdline)
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
        if (this.cur_hostIp) {
          this.getComputerList()
        }
        if (this.setting.shutdown_integration && os.platform().includes('win')) { // 需要同步关闭终端
          this.closeSocket()
          setTimeout(() => {
            ipcRenderer.send('cleanok')
          }, 500)
        }
      })
    },
    async getConsoleType (value) {
      var url
      var val = value.data.val
      var data = value.data.data
      var spicename = value.data.spicename
      var settingInfo = value.data.settingInfo
      var otherSetting = value.data.otherSetting
      if (value.type == 1) { // 使用外网
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
          url = `"spice://[${res.data.vdc_console}]` + '?' + spicename + ',' + data.spice_password + settingInfo + '"' + otherSetting
        } else {
          url = `"spice://${res.data.vdc_console}` + '?' + spicename + ',' + data.spice_password + settingInfo + '"' + otherSetting
        }
        this.getFreePort().then(port => {
          this.getCMD(val, url, port)
        })
      }
      if (value.type == 2) { // 如果是内网访问
        if (this.testIsIpv6Type(data.machine_manager_ip)) {
          url = `"spice://[${data.machine_manager_ip}]` + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + settingInfo + '"' + otherSetting
        } else {
          url = '"spice://' + data.machine_manager_ip + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + settingInfo + '"' + otherSetting
        }
        this.getFreePort().then(port => {
          this.getCMD(val, url, port)
        })
      }
      this.isShowVmConsoleTypeDialog = false
    },
    closeOptDialog () {
      this.isShowVmOptDialog = false
      this.checkAll = false
      this.isIndeterminate = false
      this.checkedVms = []
      this.getComputerList()
    },
    computerOpt (operate, vm) {
      let tipZN = ''
      let tipEN = ''
      let url = ''
      switch (operate) {
        case 'shutoff':
          tipZN = '确定关闭云服务器' + vm.name + '吗？'
          tipEN = 'Do you want to shut down cloud service ' + vm.name + ' ?'
          url = '/compute/shutoff'
          break
        case 'reboot':
          tipZN = '确定重启云服务器' + vm.name + '吗？'
          tipEN = 'Do you want to ro cloud service ' + vm.name + ' ?'
          url = '/compute/reset'
          break
        case 'start':
          tipZN = '确定启动云服务器' + vm.name + '吗？'
          tipEN = 'Do you want to start cloud service ' + vm.name + ' ?'
          url = '/compute/start'
          break
        case 'restore':
          tipZN = '确定唤醒云服务器' + vm.name + '吗？'
          tipEN = 'Do you want to restore cloud service ' + vm.name + ' ?'
          url = '/compute/restore'
          break
        case 'resume':
          tipZN = '确定恢复云服务器' + vm.name + '吗？'
          tipEN = 'Do you want to resume cloud service ' + vm.name + ' ?'
          url = '/compute/resume'
          break
      }
      this.tipContent = this.language == 'zh' ? tipZN : tipEN
      this.opt_type = operate
      this.vms_info = this.cur_vm
      this.vm_opt_params = {
        cluster_uuid: vm.cluster_uuid,
        compute_name: vm.name,
        compute_uuid: vm.uuid,
        tenant: vm.tenant
      }
      this.isShowVmOptDialog = true
    },
    // 虚拟机批量操作
    batchVmOpt (operate) {
      if (this.checkedVms.length == 0) {
        this.$message({
          message: this.$t('vm.selectVmForOpt'),
          type: 'warning',
          duration: 1500
        })
        return false
      }
      let tipZN = ''
      let tipEN = ''
      let url = ''
      switch (operate) {
        case 'bath_shutoff':
          tipZN = '确定关闭吗？'
          tipEN = 'Do you want to shut down?'
          break
        case 'bath_reboot':
          tipZN = '确定重启吗？'
          tipEN = 'Do you want to reboot?'
          break
        case 'bath_start':
          tipZN = '确定启动吗？'
          tipEN = 'Do you want to start?'
          break
        case 'bath_restore':
          tipZN = '确定唤醒吗？'
          tipEN = 'Do you want to restore?'
          break
        case 'bath_resume':
          tipZN = '确定恢复吗？'
          tipEN = 'Do you want to resume?'
          break
      }
      this.tipContent = this.language == 'zh' ? tipZN : tipEN
      this.opt_type = operate
      this.vms_info = this.checkedVms
      this.vm_opt_params = this.getBatchVmParams()
      this.isShowVmOptDialog = true
    },
    getBatchVmParams () {
      const vms = this.checkedVms.map(vm => {
        return {
          cluster_uuid: this.cluster_uuid,
          tenant: vm.tenant,
          compute_uuid: vm.uuid,
          compute_name: vm.name,
          cloud_init: true
        }
      })
      const params = {
        vms: vms,
        cluster_uuid: this.cluster_uuid
      }
      return params
    },
    getVmState (state) {
      let cur_state = ''
      switch (state) {
        case 'running':
          cur_state = this.language == 'zh' ? '运行' : 'Running'
          break
        case 'shutoff':
          cur_state = this.language == 'zh' ? '关机' : 'Shutoff'
          break
        case 'shutdown':
          cur_state = this.language == 'zh' ? '关机' : 'Shutdown'
          break
        case 'paused':
          cur_state = this.language == 'zh' ? '挂起' : 'Paused'
          break
        case 'pmsuspend':
          cur_state = this.language == 'zh' ? '睡眠' : 'Suspend'
          break
        case 'blocked':
          cur_state = this.language == 'zh' ? '阻塞' : 'Blocked'
          break
        case 'crashed':
          cur_state = this.language == 'zh' ? '崩溃' : 'Crashed'
          break
        case 'nostate':
          cur_state = this.language == 'zh' ? '无状态' : 'Nostate'
          break
      }
      return cur_state
    },
    getVmAction (action) {
      return this.language == 'zh' ? this.VSFlag.vm[action] : (action.charAt(0).toUpperCase() + action.slice(1))
    }
  },
  mounted () {
    document.getElementsByClassName('vm-checkBox')[0].classList.remove('el-checkbox-group')
    if (this.search_val) {
      this.filter_fuzzy = this.search_val
    }
    this.getComputerList()
  },
  computed: {
    ...mapState('SystemConfig', ['aesConfig'])
  },
  watch: {
    search_val (val) {
      this.filter_fuzzy = val
      this.getComputerList()
    }
  }
}
</script>
<style  lang="scss" scoped>
.desktop-seachlabel{
  padding: 10px;
  border-bottom: 1px solid #E7E7E7;
}
.active{
  background-color: #3583E3;
}
.desktop-cont{
  height: 100%;
  padding-left:30px;
  padding-right: 10px;

}
.sg-iconfont{
  cursor: pointer;
}
.sg-iconfont:hover{
  color: #3583E3;
}
.vm-opt-border{
  border-left: 1px solid #E7E7E7;border-right: 1px solid #E7E7E7;
}
.vm-list{
  height: 100%;
  .vm-cont{
    display: inline-block;margin-right:16px;    margin-top: 10px;
    /deep/.el-checkbox__input{
      position: absolute;
      top: 0;
      right: 0;
    }
    /deep/.el-checkbox__inner{
      border-radius: 10px;    width: 16px;
      height: 16px;
    }
    /deep/.el-checkbox__label{
      padding-left:0px;
    }
    .vm-pic{
      position: relative;
      height: 171px;width: 300px;
      border: 4px solid white;
      img{
        width: 300px;height: 170px;
      }
    }
    .vm-opts{
      position: absolute;
      bottom: 0px;    display: flex;
      width: 100%;    opacity: 0.7;
      height: 30%;background:rgba(50,49,50,0.66);
      i{
        color: white;
        font-size: 1.8rem;
        width: 25%;
        text-align: center;
        line-height: 190%;
      }
      i:hover{
        color:rgb(53, 131, 227);
      }
    }
    .vm-pic:hover{
      border: 4px double #A9A9A9;
    }
    .vm-pic-checked{
      border: 4px double #A9A9A9;

    }
    .select-vm{
      position: absolute;
      right: -5px;
      top: -10px;
      /deep/.el-checkbox__inner::after{
          height: 8px;
          left: 5px;
      }

    }
    span{
      display: block;
      text-align: center;
      padding-top:5px;
      padding-bottom: 10px;

    }
  }
}
.sg-iconfont:hover{
  cursor: pointer;
  color: #3583E3;
}
.vm-opts-label{
  color: #323132;    margin-top: 10px;float: right;    font-size: 16px;
  font-weight:400;
  .sg-iconfont{
    margin-left: 20px;
    margin-right: 5px;
  }
  span{
    cursor:pointer;
  }
  span:hover{
    color: #3583E3;

  }
  /deep/.el-checkbox__input{
    line-height: 2;
  }
  /deep/.el-checkbox__label{
    padding-left:5px !important;    font-size: 16px;
  }
  /deep/.el-checkbox__inner{
    border: 1px solid #323132;
    border-radius: 10px;
    width: 16px;
    height: 16px;
  }
}
.bottom-button{
    position: absolute;
    bottom: 20px;
    right: 20px;
}

.el-button{
	min-width: 72px;
	height: 32px;
	border-radius: 0px;
}
.block-desktop{
    text-align: center;
    margin-top: 18%;
    p{
      font-size: 15px;
      color: #323132;
      margin-top: 10px;
    }
}
.state-content{
    position: absolute;
    color: #E0E0E0;
    right: 10px;
}
</style>

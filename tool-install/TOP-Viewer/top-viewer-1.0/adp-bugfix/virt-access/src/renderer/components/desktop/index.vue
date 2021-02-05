<template>
  <div
    class="desktop-cont">
    <div
      v-show="computerList.length!==0"
      class="vm-opts-label">
      <el-checkbox
        v-model="checkAll"
        class="sg-iconfont"
        :indeterminate="isIndeterminate"
        @change="selectAll">
        {{ $t('resource.checkAll') }}
      </el-checkbox>
      <!-- 操作项 还原 -->
      <!-- <span
        @click="restoreVm"><i class="sg-iconfont im-icon-zhongqi" />{{ $t('cloud.restore') }}</span> -->
      <span
        @click="batchVmOpt('bath_start')"><i class="sg-iconfont im-icon-qidong" />{{ $t('cloud.start') }}</span>
      <span
        @click="batchVmOpt('bath_reboot')"><i
        class="sg-iconfont im-icon-zhongqi1"
        style="color:#666;" />{{ $t('cloud.reboot') }}</span>
      <span
        @click="batchVmOpt('bath_shutoff')"><i class="sg-iconfont im-icon-guanji" />{{ $t('cloud.close') }}</span>
      <span
        @click="getDeakVms(cur_guide_item)"><i class="sg-iconfont im-icon-shuaxin" />{{ $t('cloud.refresh') }}</span>
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
            v-if="!showBlockPic">
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
                  <li v-if="['Windows Server 2019 x64','Windows Server 2019','Windows Server 2016','Windows Server 2012','Windows Vista x64','Windows Vista x64 Edition'].includes(item.os)">
                    <img
                      v-if="item.state=='running'"
                      src="@/assets/images/vm/win.png">
                    <img
                      v-else
                      src="@/assets/images/vm/win-shutoff.png">
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
          <div
            v-else
            class="block-desktop">
            <img src="@/assets/images/icons/block_desktop.png">
            <p>{{ $t('resource.noDistribution') }}</p>
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
      :desktopFlag="true"
      :isShowVmOptDialog="isShowVmOptDialog"
      @closeOptDialog="closeOptDialog" />
    <!-- <vs-loading
      :is-show="onLoading" /> -->
    <vm-console-type-dialog
      v-if="isShowVmConsoleTypeDialog"
      :type_dialog_val="type_dialog_val"
      :isShowVmConsoleTypeDialog="isShowVmConsoleTypeDialog"
      @getConsoleType="getConsoleType" />
  </div>
</template>

<script>
import { ipcRenderer } from 'electron'
import { mapState, mapMutations, mapActions } from 'vuex'
import Basic from '@/test/basic.js'
import systemSettingDialog from '../common/systemSettingDialog'
import vmOptDialog from '../common/vmOptDialog'
import vmConsoleTypeDialog from '../common/vmConsoleTypeDialog'
import { decryptAESPassword } from '@/components/common/js/aesPassword'
import ElectronStore from 'electron-store'
const exec = require('child_process').exec
var os = require('os')
var electronStore = new ElectronStore() // 存储数据
export default {
  mixins: [Basic],
  components: {systemSettingDialog, vmOptDialog, vmConsoleTypeDialog},
  data () {
    return {
      // startVms :[],
      showBlockPic: false,
      desktopValue: '',
      filter_fuzzy: '',
      computerList: [],
      cur_computer: {
        name: ''
      },
      cur_vm: '',
      desktop_setInfo: '',
      showSetting: false,
      checkedVms: [],
      checkAll: false,
      isIndeterminate: false,
      reGetVms: null, // 定时任务 刷克隆状态
      isShowVmOptDialog: false,
      isShowVmConsoleTypeDialog: false,
      type_dialog_val: '',
      tipContent: '',
      opt_type: '',
      vms_info: '',
      vm_opt_params: '',
      loading: '',
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
      this.checkedVms = value ? this.computerList.filter(item => item.action == 'noaction') : []
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
    getDeakVms (val) {
      const param = {
        exclusive_desktop_uuid: val.exclusive_desktop_uuid,
        cluster_uuid: val.cluster_uuid,
        tenant: val.tenant,
        agent_client_id: this.hostId
      }
      this.setState({
        attr: 'onLoading',
        val: true
      })
      if (this.loading) {
        this.loading.close()
        this.loading = ''
      }
      // eslint-disable-next-line standard/object-curly-even-spacing
      this.loading = this.$loading({ target: '.desktop-cont'})
      this.checkedVms = []
      this.checkAll = false
      clearInterval(this.reGetVms)
      this.reGetVms = null
      this.$Api.desktop.desktopInfo(param).then(res => {
        this.loading.close()
        if (res.scode == 0) {
          this.desktop_setInfo = res.data.personalized_strategy.uuid ? res.data.personalized_strategy : '' // 对虚拟机的个性化设置
          this.computerList = res.data.vm_list
          let flag = 0
          // 遍历 看是否有克隆中状态的，如果有 需要实时刷新数据
          this.computerList.forEach(item => {
            if (item.action == 'cloning' || item.action == 'waiting') {
              flag = 1
            }
            // 动作 0: noaction, 1: cloning, 2: save, 3: migrating, 4: pending, 5: importing 6.unknown(未知) 7.importSuspend
            item.action = item.action == 'importing' && item.attr && item.attr.ImportSuspend ? 'importSuspend' : item.action
            // item.flagtxt = this.language == 'zh' ? this.VSFlag.vm[item.action] : item.action
          })
          if (flag == 0) {
            clearInterval(this.reGetVms)
            this.reGetVms = null
          } else {
            this.reGetVms = setInterval(() => {
              this.getDeakVms(this.cur_guide_item)
            }, 10000)
          }
        } else {
          this.computerList = []
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
        if (this.computerList.length == 0) {
          this.showBlockPic = true
        } else {
          this.showBlockPic = false
        }
      }).catch(() => {
        this.loading.close()
      })
    },
    getComputerOpt (val) {
      this.cur_computer = val
    },
    checkConsoleState (val) { // 打开控制台前，检查状态
      // 操作前先判定是否可以操作
      var flag = this.bindVms.length == 0
      var agentFlag = !((val.vdc_agent_list !== undefined && val.vdc_agent_list && val.vdc_agent_list.length !== 0))
      var poolFlag
      this.bindVms.forEach(pool => {
        if (pool[0] == this.cur_guide_item.exclusive_desktop_uuid) { // 先比较池ID
          poolFlag = true // 当前池含在已绑定的池中
          pool[1].forEach(bind_vm => {
            if (bind_vm == val.uuid) {
              flag = true
            }
          })
        }
      })
      // 有绑定了池，但当前池不属于被绑定的
      if (this.bindVms.length > 0) {
        if (poolFlag !== true) {
          flag = false
        }
      }
      if (val.vdc_agent_list === undefined) { // 如果不存在这个字段，兼容之前的vdc_agent数据、
        if (val.vdc_agent) {
          if (this.agentNameId != val.vdc_agent) {
            agentFlag = false
          } else {
            agentFlag = true
            flag = true
          }
        }
      } else {
        val.vdc_agent_list && val.vdc_agent_list.forEach(agent => {
        // 遍历绑定的客户机
          if (this.agentNameId == agent) {
            // 如果绑定了客户机 并且当前客户机id与绑定值不一样
            // 就不能操作
            agentFlag = true
          }
        })
      }
      if ((this.bindVms.length == 0 && agentFlag === false) || (this.bindVms.length > 0 && flag !== true && agentFlag !== true) || (!val.vdc_agent_list && flag !== true)) {
        this.$message({
          message: this.$t('cloud.noAuthOpt'),
          type: 'warning',
          duration: 1500
        })
        return false
      }
      // val.state == 'pmsuspend'
      if (val.state == 'shutoff' || val.action == 'save' || val.state == 'paused') { // 如果是关机 就开启;如果休眠 就唤醒
        this.$nprogress.start()
        let Api = val.state == 'shutoff' ? this.$Api.desktop.desktopStartVm : val.action == 'save' ? this.$Api.computers.vmRestore : this.$Api.computers.vmResume
        let params = {
          cloud_init: true,
          cluster_uuid: val.cluster_uuid,
          compute_name: val.name,
          compute_uuid: val.uuid,
          tenant: val.tenant
        }
        if (val.state == 'shutoff') { // 启动的参数要另外处理
          params.exclusive_desktop_name = this.cur_guide_item.exclusive_desktop_name
          params.exclusive_desktop_uuid = this.cur_guide_item.exclusive_desktop_uuid
          params.vms = [
            {compute_name: val.name,
              compute_uuid: val.uuid } ]
        }
        Api(params).then(res => {
          this.$nprogress.done()
          if (res.scode == 0) { // 开机成功，调用虚拟机
            this.getDeakVms(this.cur_guide_item)
            this.getConsole(val)
          } else {
            this.$message({
              message: res.message_cn,
              type: 'error',
              duration: 3000
            })
          }
        }).catch((error) => {
          this.$nprogress.done()
          if (error.response.status == 404 && val.state == 'shutoff') {
            this.dealVmStartApi(params, val)
          }
        })
      } else {
        this.getConsole(val)
      }
    },
    dealVmStartApi (params, val) {
      this.$Api.desktop.desktopBatchStartVm(params).then(res => {
        if (res.data.fail > 0) {
          this.$message({
            message: this.language == 'zh' ? res.data.results[0].message_cn : res.data.results[0].message,
            type: 'warning',
            duration: 4500
          })
        } else {
          this.$message({
            message: this.$t('disk.operationSuccess'),
            type: 'success',
            duration: 1500
          })
          this.getDeakVms(this.cur_guide_item)
          this.getConsole(val)
        }
      })
    },
    // 桌面控制 打开VM
    getConsole (val) {
      // if (val.state != 'running' && val.state != 'suspend') {
      //   this.$message({
      //     message: `${this.$t('vm.startFirstly')}`,
      //     type: 'warning',
      //     duration: 3000
      //   })
      //   return false
      // }
      var param = {
        compute_uuid: val.uuid,
        tenant: val.tenant,
        cluster_uuid: val.cluster_uuid ? val.cluster_uuid : this.cluster_uuid
      }
      this.$nprogress.start()
      this.$Api.computers.vmInfo(param).then(res => {
        this.$nprogress.done()
        if (res.scode == 0) {
          if (res.data.machine_manager_ip) {
            this.openConsole(val, res.data) // 打开控制台
            // 桌面资源池中虚拟机注销还原(用户使用虚拟机前调用)
          } else {
            this.$message({
              message: '无可用IP信息',
              type: 'error',
              duration: 3000
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
    async openConsole (val, data) {
      // 拼接桌面池设置信息
      // Viewer命令行加水印的格式：
      // 暂时设定9个参数：内容、移动时间、倾斜类型、字体大小、字体粗细、字体颜色、字体类型、下划线、背景色
      // ex:  spice://10.30.10.21:5931?zkm,FakePasswd#zkm,3(水印位置刷新时间(单位s),为0为静止水印),normal,20,40,red,monospace,low,red
      // 倾斜类型选项：normal、oblique、italic（后两者区别不大）
      // 字体颜色选项：black、white、red...
      // 字体类型选型：normal、sans、serif、monospace
      // 下划线选项：single、double、low、none
      // 背景色：black、white、red...
      let settingInfo = ''
      if (this.setting.uuid && this.setting.enable_screen_watermark && this.setting.watermark_content) { // 水印是开启状态，并且水印有内容
        settingInfo = '#' + this.setting.watermark_content + ',' + this.setting.watermark_move_time + ',' + (this.setting.lean ? this.setting.lean : 'normal') + ',' + this.setting.font_size + ',' + this.setting.font_thickness + ',' + this.setting.content_color + ',' + this.setting.show_style + ',' + (this.setting.underline_type ? this.setting.underline_type : 'none') + ',' + (this.setting.background_color ? this.setting.background_color !== 'transparent' ? (this.setting.background_color + ',0') : 'pink,1.0' : 'none') + ',' + this.setting.watermark_coordinate_x + ',' + this.setting.watermark_coordinate_y
      } else if (this.desktop_setInfo.uuid && this.desktop_setInfo.enable_screen_watermark && this.desktop_setInfo.watermark_content) {
        settingInfo = '#' + this.desktop_setInfo.watermark_content + ',' + this.desktop_setInfo.watermark_move_time + ',' + (this.desktop_setInfo.lean ? this.desktop_setInfo.lean : 'normal') + ',' + this.desktop_setInfo.font_size + ',' + this.desktop_setInfo.font_thickness + ',' + this.desktop_setInfo.content_color + ',' + this.desktop_setInfo.show_style + ',' + (this.desktop_setInfo.underline_type ? this.desktop_setInfo.underline_type : 'none') + ',' + (this.desktop_setInfo.background_color ? this.desktop_setInfo.background_color !== 'transparent' ? (this.desktop_setInfo.background_color + ',0') : 'pink,1.0' : 'none') + ',' + this.desktop_setInfo.watermark_coordinate_x + ',' + this.desktop_setInfo.watermark_coordinate_y
      }

      // 如果vuex里的poolSetting有值,遍历它,看是否实时修改了启用的桌面池的设置
      if (this.poolSetting) {
        this.poolSetting.associated_desktop_pool.forEach(pool => {
          if (pool.desktop_pool_uuid == this.cur_guide_item.exclusive_desktop_uuid) { // 如果修改包含选中的池
            settingInfo = '#' + this.poolSetting.watermark_content + ',' + this.poolSetting.watermark_move_time + ',' + (this.poolSetting.lean ? this.poolSetting.lean : 'normal') + ',' + this.poolSetting.font_size + ',' + this.poolSetting.font_thickness + ',' + this.poolSetting.content_color + ',' + this.poolSetting.show_style + ',' + (this.poolSetting.underline_type ? this.poolSetting.underline_type : 'none') + ',' + (this.poolSetting.background_color ? this.poolSetting.background_color !== 'transparent' ? (this.poolSetting.background_color + ',0') : 'pink,1.0' : 'none') + ',' + this.poolSetting.watermark_coordinate_x + ',' + this.poolSetting.watermark_coordinate_y
          }
        })
      }
      // 全屏等设置信息
      // 禁止输入的暂时去掉+ (this.cur_guide_item.ban_input ? '' : '-d ')
      // allow_usb_redirect USB重定向
      // drag_drop_files 允许拖拽 禁用：--disable-drag-file
      // shared_clipboard 共享剪切板 禁用：--disable-auto-clipboard
      // 如果是开启了全屏，就不拼接分辨率，不然topviewer的显示可能会有问题

      // 设置有安全策略设置 个性化设置 桌面池设置,优先级逐步降低
      let otherSetting = ''
      if (this.setting.uuid) { // 存在用户设置
        otherSetting = ' ' + (this.cur_guide_item.start_full_screen ? '' : this.cur_guide_item.resolution.replace('x', '*') + ' ') +
        ((this.setting.allow_usb_redirect === false) ? '--disable-usb-usbredir --spice-usbredir-auto-redirect-filter="-1,-1,-1,-1,0" ' : '') +
        (((this.setting.drag_drop_files === false && !this.securitySetting) || this.securitySetting.drag_file === false) ? '--disable-drag-file ' : '') +
        (((this.setting.allow_pc_clipboard === false && !this.securitySetting) || this.securitySetting.share_clipboard === false) ? '--disable-auto-clipboard ' : '') +
        (this.cur_guide_item.start_full_screen ? '-f ' : '') + '-p ' + this.cur_guide_item.image_compression + ' -c ' + this.cur_guide_item.video_compression
      } else {
        // 如果没有用户设置，先看有没有对这台客户机 以及该触发的虚拟机进行的设置
        if (this.desktop_setInfo.uuid) {
          otherSetting = ' ' + (this.cur_guide_item.start_full_screen ? '' : this.cur_guide_item.resolution.replace('x', '*') + ' ') +
              (this.desktop_setInfo.allow_usb_redirect === false ? '--disable-usb-usbredir --spice-usbredir-auto-redirect-filter="-1,-1,-1,-1,0" ' : '') +
              (((this.desktop_setInfo.drag_drop_files === false && !this.securitySetting) || this.securitySetting.drag_file === false) ? '--disable-drag-file ' : '') +
              (((this.desktop_setInfo.allow_pc_clipboard === false && !this.securitySetting) || this.securitySetting.share_clipboard === false) ? '--disable-auto-clipboard ' : '') +
              (this.desktop_setInfo.start_full_screen ? '-f ' : '') + '-p ' + this.cur_guide_item.image_compression + ' -c ' + this.cur_guide_item.video_compression
        } else {
          otherSetting = ' ' + (this.cur_guide_item.start_full_screen ? '' : this.cur_guide_item.resolution.replace('x', '*') + ' ') +
          (((this.cur_guide_item.drag_drop_files === false && !this.securitySetting) || this.securitySetting.drag_file === false) ? '--disable-drag-file ' : '') +
          (((this.cur_guide_item.shared_clipboard === false && !this.securitySetting) || this.securitySetting.share_clipboard === false) ? '--disable-auto-clipboard ' : '') +
          (this.cur_guide_item.start_full_screen ? '-f ' : '') + '-p ' + this.cur_guide_item.image_compression + ' -c ' + this.cur_guide_item.video_compression
        }
      }

      const spicename = encodeURIComponent(data.name)
      data.spice_password && (data.spice_password = decryptAESPassword(data.spice_password, this.aesConfig.aesIv, this.aesConfig.aesKey))
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
          url = `"spice://[${data.machine_manager_ip}]` + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + settingInfo + '"' + otherSetting
        } else {
          url = '"spice://' + data.machine_manager_ip + ':' + data.spice_port + '?' + spicename + ',' + data.spice_password + settingInfo + '"' + otherSetting
        }
        this.getFreePort().then(port => {
          this.getCMD(val, url, port)
        })
      }
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
        vmId: val.uuid,
        vmName: val.name,
        tenant: val.tenant,
        exclusive_desktop_uuid: this.cur_guide_item.exclusive_desktop_uuid,
        setting_uuid: this.setting.uuid ? this.setting.uuid : this.desktop_setInfo.uuid, // 设置的id
        shutdownFlag: this.setting.uuid ? this.setting.shutdown_integration : this.desktop_setInfo.shutdown_integration // 关闭虚拟机时 是否关闭程序
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
      console.log('line', cmdline)
      exec(cmdline, (error, stdout, stderr) => {
        if (error && this.cur_hostIp) {
          this.$message({
            message: this.$t('vm.startFailed'),
            type: 'error',
            duration: 3000
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
        // 关闭后刷新当前页面
        if (this.cur_hostIp) {
          this.getDeakVms(this.cur_guide_item)
        }
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
      this.getDeakVms(this.cur_guide_item)
    },
    computerOpt (operate, vm) {
      // 操作前先判定是否可以操作
      var flag = this.bindVms.length == 0
      var agentFlag = !((vm.vdc_agent_list !== undefined && vm.vdc_agent_list && vm.vdc_agent_list.length !== 0))
      var poolFlag
      this.bindVms.forEach(pool => {
        if (pool[0] == this.cur_guide_item.exclusive_desktop_uuid) { // 先比较池ID
          poolFlag = true // 当前池含在已绑定的池中
          pool[1].forEach(bind_vm => {
            if (bind_vm == vm.uuid) {
              flag = true
            }
          })
        }
      })
      // 有绑定了池，但当前池不属于被绑定的
      if (this.bindVms.length > 0) {
        if (poolFlag !== true) {
          flag = false
        }
      }
      if (vm.vdc_agent_list === undefined) { // 如果不存在这个字段，兼容之前的vdc_agent数据、
        if (vm.vdc_agent) {
          if (this.agentNameId != vm.vdc_agent) {
            agentFlag = false
          } else {
            agentFlag = true
            flag = true
          }
        }
      } else {
        vm.vdc_agent_list && vm.vdc_agent_list.forEach(agent => {
        // 遍历绑定的客户机
          if (this.agentNameId == agent) {
            // 如果绑定了客户机 并且当前客户机id与绑定值不一样
            // 就不能操作
            agentFlag = true
          }
        })
      }
      if ((this.bindVms.length == 0 && agentFlag === false) || (this.bindVms.length > 0 && flag !== true && agentFlag !== true) || (!vm.vdc_agent_list && flag !== true)) {
        this.$message({
          message: this.$t('cloud.noAuthOpt'),
          type: 'warning',
          duration: 1500
        })
        return false
      }
      let tipZN = ''
      let tipEN = ''
      let url = ''
      switch (operate) {
        case 'shutoff':
          tipZN = '确定关闭云桌面' + vm.name + '吗？'
          tipEN = 'Do you want to shut down cloud desktop ' + vm.name + ' ?'
          url = '/compute/shutoff'
          break
        case 'reboot':
          tipZN = '确定重启云桌面' + vm.name + '吗？'
          tipEN = 'Do you want to ro cloud desktop ' + vm.name + ' ?'
          url = '/compute/reset'
          break
        case 'start':
          tipZN = '确定启动云桌面' + vm.name + '吗？'
          tipEN = 'Do you want to start cloud desktop ' + vm.name + ' ?'
          url = '/compute/start'
          break
        case 'restore':
          tipZN = '确定唤醒云桌面' + vm.name + '吗？'
          tipEN = 'Do you want to restore cloud desktop ' + vm.name + ' ?'
          url = '/compute/restore'
          break
        case 'resume':
          tipZN = '确定恢复云桌面' + vm.name + '吗？'
          tipEN = 'Do you want to resume cloud desktop ' + vm.name + ' ?'
          url = '/compute/resume'
          break
      }
      this.tipContent = this.language == 'zh' ? tipZN : tipEN
      this.opt_type = operate
      this.vms_info = this.cur_vm
      if (operate == 'start') {
        this.vm_opt_params = {
          cluster_uuid: this.cluster_uuid,
          exclusive_desktop_uuid: this.cur_guide_item.exclusive_desktop_uuid,
          exclusive_desktop_name: this.cur_guide_item.exclusive_desktop_name,
          vms: [{
            compute_name: vm.name,
            compute_uuid: vm.uuid

          }],
          tenant: vm.tenant
        }
      } else {
        this.vm_opt_params = {
          cluster_uuid: this.cluster_uuid,
          exclusive_desktop_uuid: this.cur_guide_item.exclusive_desktop_uuid,
          exclusive_desktop_name: this.cur_guide_item.exclusive_desktop_name,
          compute_name: vm.name,
          compute_uuid: vm.uuid,
          tenant: vm.tenant
        }
      }
      this.isShowVmOptDialog = true
    },
    // 虚拟机批量操作
    batchVmOpt (operate) {
      if (this.checkedVms.length == 0) {
        this.$message({
          message: this.$t('vm.selectDesktopForOpt'),
          type: 'warning',
          duration: 1500
        })
        return false
      }
      let tipZN = ''
      let tipEN = ''
      let url = ''
      this.opt_type = operate
      this.vms_info = this.checkedVms
      var canContinue
      var flag
      this.vms_info.forEach(selected => { // 遍历选中的虚拟机
        flag = this.bindVms.length == 0 ? true : null
        var vmFlag = false
        var poolFlag = this.bindVms.length == 0 ? true : null // 用于比较是否属于绑定的池ID
        // 当前客户机如果已经绑定了虚拟机
        // 先比较选中的是否属于绑定的虚拟机
        // 再比较选中的虚拟机是否绑定了当前客户机
        // 两个条件满足任一，就可以操作
        this.bindVms.forEach(pool => {
          if (pool[0] == this.cur_guide_item.exclusive_desktop_uuid) { // 先比较池ID
            poolFlag = true
            vmFlag = false
            pool[1].forEach(bind_vm => {
              if (bind_vm == selected.uuid) { // 选中的虚拟机允许被操作
                vmFlag = true
              }
            })
            if (vmFlag !== true) {
              // 绑定的虚拟机，没有与选中的内容uuid相同的，就不能操作
              //  此时要查看这些选中的虚拟机是否绑定了客户机
              if (selected.vdc_agent_list && selected.vdc_agent_list.length !== 0) {
                // 当前选择的虚拟机存在被绑定的客户机，遍历
                flag = 'no'
                selected.vdc_agent_list.forEach(agent => {
                  // 遍历绑定的客户机
                  if (this.agentNameId == agent) {
                    // 如果绑定了客户机 并且当前客户机id与绑定值不一样
                    // 就不能操作
                    flag = true
                  }
                })
              } else {
                flag = 'no'
              }
              // if (selected.vdc_agent) {
              //   if (selected.vdc_agent !== this.agentNameId) {
              //     flag = 'no'
              //   }
              //   if (selected.vdc_agent == this.agentNameId) {
              //     flag = true
              //   }
              // } else {
              //   // 绑定了池但不属于绑定，选择的内容也没有绑定客户机，则不能操作
              //   flag = 'no'
              // }
            }
          }
        })
        //
        if (poolFlag !== true) {
          // 当前选择的池不属于被绑定的池，则需要检查选中的虚拟机绑定了当前客户机
          // 如果都没有，则不能操作
          poolFlag = false
          if (selected.vdc_agent_list && selected.vdc_agent_list.length !== 0) {
            // 当前选择的虚拟机存在被绑定的客户机，遍历
            flag = 'no'
            selected.vdc_agent_list.forEach(agent => {
              // 遍历绑定的客户机
              if (this.agentNameId == agent) {
                // 如果绑定了客户机 并且当前客户机id与绑定值不一样
                // 就不能操作
                flag = true
              }
            })
          } else {
            flag = 'no'
          }
        }
        if (this.bindVms.length == 0) {
          // 如果没有绑定池，遍历选择的内容是否绑定了客户机
          if (selected.vdc_agent_list && selected.vdc_agent_list.length !== 0) {
            // 当前选择的虚拟机存在被绑定的客户机，遍历
            flag = 'no'
            selected.vdc_agent_list.forEach(agent => {
              // 遍历绑定的客户机
              if (this.agentNameId == agent) {
                // 如果绑定了客户机 并且当前客户机id与绑定值不一样
                // 就不能操作
                flag = true
              }
            })
          }
        }
        if (flag == 'no') {
          canContinue = false
          setTimeout(() => {
            this.$message({
              message: this.$t('cloud.noAuthOpt') + selected.name,
              type: 'warning',
              duration: 1500
            })
          }, 1500)
        }
      })
      if (canContinue === false) {
        return
      }
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
      this.vm_opt_params = this.getBatchVmParams(operate)
      this.isShowVmOptDialog = true
    },
    restoreVm () {
      if (this.setting.restore) {

      }
    },
    getBatchVmParams (operate) {
      var params
      let vms = this.checkedVms.map(vm => {
        return {
          cluster_uuid: vm.cluster_uuid,
          tenant: vm.tenant,
          compute_uuid: vm.uuid,
          compute_name: vm.name,
          cloud_init: true
        }
      })
      if (operate == 'bath_start') {
        params = {
          vms: vms,
          cluster_uuid: this.cluster_uuid,
          exclusive_desktop_uuid: this.cur_guide_item.exclusive_desktop_uuid,
          exclusive_desktop_name: this.cur_guide_item.exclusive_desktop_name

        }
      } else {
        params = {
          vms: vms
        }
      }
      return params
    },
    getVmState (state) {
      let cur_state = ''
      // running: '运行',
      // shutdown: '关机',
      // nostate: '无状态',
      // blocked: '阻塞',
      // crashed: '崩溃',
      // shutoff: '关机',
      // pmsuspend: '睡眠',
      // paused: '挂起',
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
  computed: {
    ...mapState('SystemConfig', ['aesConfig', 'hostId', 'agentNameId']),
    ...mapState('Computers', ['cur_StartedVmList'])
  },
  watch: {
    cur_guide_item (val) {
      this.showimage = ''
      if (!val) return false
      if (val == 'block_res') {
        this.computerList = []
        this.showBlockPic = true
      } else {
        this.getDeakVms(val)
      }
    },
    reFresh_cur_page (val) {
      this.getDeakVms(this.cur_guide_item)
    }
  },
  created () {
    if (sessionStorage.getItem('auto_open_vm')) {
      setTimeout(() => { // 打开第一台虚拟机
        if (this.computerList.length > 0 && !(!this.cur_guide_item || JSON.stringify(this.cur_guide_item) == '{}')) {
          this.checkConsoleState(this.computerList[0])
        }
        sessionStorage.removeItem('auto_open_vm')
      }, this.setting.auto_login_time * 1000)
    }
  },
  mounted () {
    document.getElementsByClassName('vm-checkBox')[0].classList.remove('el-checkbox-group')
    if (!this.cur_guide_item || JSON.stringify(this.cur_guide_item) == '{}' || this.cur_guide_item == 'block_res') {
      this.showBlockPic = true
      return false
    }
    this.getDeakVms(this.cur_guide_item)
  },
  destroyed () {
    clearInterval(this.reGetVms)
    this.reGetVms = null
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
        width: 300px;height: 170px;cursor:pointer;
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
    .vm-error{
      position: absolute;
      color: white;
      width: 100%;
      height: 100%;background-color: #272727;
      text-align: center;
      .vm-error-cont{
        text-align: left;
        padding-left: 30px;
        padding-top: 50px;
        span{
          display: inline-block;
          word-break: break-all;
          text-align: left;
        }
      }
      .vm-cloning-cont{
        padding-top: 70px;
      }
    }
  }
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

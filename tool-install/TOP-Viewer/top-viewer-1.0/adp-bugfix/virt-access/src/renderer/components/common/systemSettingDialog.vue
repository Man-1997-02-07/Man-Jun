<template>
  <div>
    <el-dialog
      :title="`${$t('resource.setting')}`"
      :visible.sync="visible"
      :close-on-click-modal="false"
      :modal-append-to-body="false"
      width="685px"
      @close="closeSettingDialog">
      <div
        id="host_normal_setting"
        class="dialog-cont">
        <el-radio-group
          v-model="tabType"
          style="margin-bottom: 30px;">
          <el-radio-button label="size">
            {{ $t('setting.hostSetting') }}
          </el-radio-button>
          <el-radio-button label="net">
            {{ $t('setting.netSetting') }}
          </el-radio-button>
          <el-radio-button label="netTest">
            {{ $t('setting.netProde') }}
          </el-radio-button>
        </el-radio-group>
        <div
          v-show="tabType=='size'"
          class="input-cont"
          style="height: 385px;">
          <!-- <el-form
            label-width="85px">
            <el-form-item
              :label="$t('setting.resolution')">
              <el-select
                id="setting_select_resolution"
                v-model="resolution"
                style="width:380px;"
                :placeholder="$t('setting.selectResolution')">
                <el-option
                  v-for="(item,index) in resolutionList"
                  :key="index"
                  :label="item"
                  :value="item" />
              </el-select>
            </el-form-item>
          </el-form> -->
          <!-- 主机设置 -->
          <div
            :class="language=='zh'?'host-opt-cont':'host-opt-cont-en'">
            <p>{{ $t('setting.hostopt') }}</p>
            <div>
              <el-button
                id="setting_closeHost"
                @click="closeHost">
                {{ $t('setting.hostShutdown') }}
              </el-button>
              <span> {{ $t('setting.shutdownTip') }}</span>
            </div>
            <div>
              <el-button @click="restartHost">
                {{ $t('setting.hostRestart') }}
              </el-button><span>{{ $t('setting.restartTip') }}</span>
            </div>
            <!-- 权限拓展 -->
            <div v-if="isWin">
              <el-button
                id="setting_useManagerRole"
                @click="useManagerRole">
                {{ $t('setting.useManager') }}
              </el-button><span :style="language=='en'?'line-height:19px':''">{{ $t('setting.getManagerRole') }}</span>
            </div>
            <!-- 启动agent -->
            <div>
              <el-button
                id="setting_startAgent"
                @click="startAgent">
                {{ $t('setting.startAgent') }}
              </el-button><span>{{ $t('setting.startAgentTip') }}</span>
            </div>
          </div>
          <div
            :class="language=='zh'?'host-opt-cont':'console-opt-cont-en'">
            <p style="margin-top: 30px;">
              <!-- 其他设置 -->
              {{ $t('setting.otherSetting') }}
            </p>
            <!--控制台访问-->
            <div>
              <el-button @click="consoleSet">
                {{ $t('system.console') }}
              </el-button><span>  {{ $t('system.consoleDesc') }}</span>
            </div>
            <p class="tip">
              {{ $t('system.curConsoleSetting') }}{{ consoleSettingType }}
            </p>
            <div>
              <el-button @click="autoLogin">
                <!-- 自动登录 -->
                {{ $t('setting.autoLogin') }}
              </el-button><span>{{ $t('setting.autoLoginAfter5s') }}</span>
            </div>
            <p
              class="tip"
              style="padding-left: 200px;">
              {{ isSettingAutoLogin }}
            </p>
          </div>
        </div>

        <!-- 网络设置 -->
        <div
          v-show="tabType=='net'"
          class="input-cont net-cont">
          <el-form
            ref="net"
            :rules="netRules"
            :model="net"
            label-position="left"
            :label-width="$i18n.locale == 'zh'?'135px':'171px'">
            <el-radio-group
              v-model="ipType"
              @change="ipTypeSetting(ipType)">
              <!-- 自动获取地址/使用下面的地址 -->
              <el-radio label="1">
                {{ $t('setting.getAutoIp') }}
              </el-radio>
              <el-radio label="2">
                {{ $t('setting.useSavedIP') }}
              </el-radio>
            </el-radio-group>
            <el-form-item
              v-if="netInfoRes.length!==0"
              prop="cur_netcpaor"
              :label="$t('setting.netcard')">
              <el-select
                id="netInfoRes_cur_netcard"
                v-model="cur_netcard"
                @change="changeNetcard">
                <el-option
                  v-for="(item,index) in netInfoRes"
                  :key="index"
                  :label="item.Name"
                  :value="item.Name" />
              </el-select>
            </el-form-item>
            <el-form-item
              prop="Ip"
              :label="$t('setting.ipAddress')">
              <el-input
                v-model="net.Ip"
                :disabled="ipType==1" />
            </el-form-item>
            <el-form-item
              prop="Netmask"
              :label="$t('setting.mask')">
              <el-input
                v-model="net.Netmask"
                :disabled="ipType==1" />
            </el-form-item>
            <el-form-item
              prop="Gateway"
              :label="$t('setting.defaultGateway')">
              <el-input
                v-model="net.Gateway"
                :disabled="ipType==1" />
            </el-form-item>
            <!-- <el-radio-group
              v-model="ipType"
              @change="ipTypeSetting(ipType)">
              <el-radio label="1">
                {{ $t('setting.getAutoDNS') }}
              </el-radio>
              <el-radio label="2">
                {{ $t('setting.useSavedDNS') }}
              </el-radio>
            </el-radio-group> -->
            <el-form-item
              prop="fService"
              :label="$t('setting.firstDNS')">
              <el-input
                v-model="net.fService"
                :disabled="ipType==1" />
            </el-form-item>
            <el-form-item
              prop="backupService"
              :label="$t('setting.secondDNS')">
              <el-input
                v-model="net.backupService"
                :disabled="ipType==1" />
            </el-form-item>
          </el-form>
        </div>
        <div
          v-show="tabType=='netTest'"
          class="input-cont">
          <el-form
            ref="netTest"
            :rules="rules"
            :model="netTest"
            label-position="left">
            <el-form-item
              prop="Addr"
              class="netTest-left-label"
              style="display: inline-block;"
              :label="$t('setting.ipAddress')">
              <el-input
                id="netTest_ip"
                v-model="netTest.Addr"
                style="width:270px;" />
            </el-form-item>
            <el-form-item
              prop="Count"
              class="netTest-right-label"
              style="display: inline-block;"
              :label="$t('setting.count')">
              <el-input
                v-model="netTest.Count"
                style="width: 130px;" />
            </el-form-item>
            <el-form-item
              class="netTest-left-label"
              :label="$t('setting.output')">
              <el-input
                id="netTest_output"
                v-model="netTest.PingText"
                style="width: 482px;"
                type="textarea"
                :rows="10" />
            </el-form-item>
          </el-form>
        </div>
        <div class="net-button-label">
          <div>
            <el-button
              v-if="tabType!=='size'"
              id="setting_confirm_btn"
              type="primary"
              @click="getSetting(tabType)">
              {{ $t('resource.confirm') }}
            </el-button>
            <el-button @click="closeSettingDialog">
              {{ $t('resource.close') }}
            </el-button>
          </div>
        </div>
      </div>
      <!-- <vs-loading
        :is-show="innerLoading"
        class-name="vs-inner-loading" />
    </el-dialog> -->
      <vm-console-type-dialog
        v-if="isShowVmConsoleTypeDialog"
        :isShowVmConsoleTypeDialog="isShowVmConsoleTypeDialog"
        :type_dialog_val="'systemSetting'"
        @closeConsoleType="closeConsoleType"
        @getConsoleType="getConsoleType" />
    </el-dialog>
  </div>
</template>
<script>
import Basic from '@/test/basic.js'
import {mapState, mapMutations} from 'vuex'
import vmConsoleTypeDialog from './vmConsoleTypeDialog'
import ElectronStore from 'electron-store'
var store = new ElectronStore() // 存储数据
var os = require('os')
var iconv = require('iconv-lite')
const exec = require('child_process').exec
export default {
  mixins: [Basic],
  props: ['show'],
  components: {vmConsoleTypeDialog},
  data () {
    return {
      isWin: true,
      isShowVmConsoleTypeDialog: false,
      consoleSettingType: '',
      loadingHidden: '',
      visible: this.show,
      tabType: 'size',
      ipType: '1',
      settingForm: {},
      resolution: '',
      // 网络探测的参数
      netTest: {
        Count: '',
        Addr: '',
        PingText: ''
      },
      rules: {
        Addr: [ {
          required: true,
          validator: this.testAddress('IP'),
          trigger: 'change'
        }],
        Count: [{required: true, validator: this.testCount(this.$t('setting.count')), rigger: 'change'}]
      },
      netRules: {
        Ip: [{ required: true, validator: this.testAddress('IP：'), trigger: 'change' }],
        Netmask: [{ required: true, validator: this.testMask(this.$t('setting.mask')), trigger: 'change' }],
        Gateway: [{ required: true, validator: this.testAddress(this.$t('setting.defaultGateway')), trigger: 'change' }],
        fService: [{ required: true, validator: this.testAddress(this.$t('setting.firstDNS')), trigger: 'change' }],
        backupService: [{ required: true, validator: this.testAddress(this.$t('setting.secondDNS')), trigger: 'change' }]
      },
      netWebsocket: '',
      net: {
        ipType: '',
        name: '',
        Macaddr: '',
        Ip: '',
        Netmask: '',
        Gateway: '',
        fService: '',
        backupService: ''
      },
      netcardList: [],
      netInfoRes: [], // 网络设置的返回结果
      cur_netcard: [],
      cur_netInfo: '',
      resolutionList: [],
      getAgentState: null,
      isSettingAutoLogin: ''
    }
  },
  computed: {
    ...mapState('SystemConfig', ['hostId', 'isAdminRoler'])
  },
  methods: {
    ...mapMutations('SystemConfig', ['setHostInfos']),
    getSetting (type) {
      switch (type) {
        case 'size':
          // this.submitResolution()
          break
        case 'net':
          this.submitNet()
          break
        case 'netTest':
          this.submitNetTest()
          break
      }
    },
    closeSettingDialog () {
      this.$emit('closeSettingDialog')
    },
    startAgent () {
      let cmdline
      if (this.isWin) {
        cmdline = '"C:\\Program Files\\TopViewer1.0\\vdiagent.exe"'
      } else {
        cmdline = '/usr/local/bin/vdiagent topmonitor start --conf /var/lib/vdiagent/vdiagent.conf'
      }
      exec(cmdline, {encoding: 'gbk'}, (error, stdout, stderr) => {
        let errMsg = 'Error: Command failed: C:\Program Files\TopViewer1.0\vdiagent.exe C:\Program Files\TopViewer1.0\vdiagent.exe �����ڲ����ⲿ���Ҳ���ǿ����еĳ������������ļ���'
        // console.log('错误信息', iconv.decode(Buffer.from(iconv.decode(Buffer.from(errMsg), 'gbk')), 'utf8'))
        // console.log('错误', error)
        if (error) {
          this.$message({
            message: error,
            type: 'error',
            duration: 5000
          })
        } else {
          this.$message({
            message: this.$t('setting.startSuccess'),
            type: 'success',
            duration: 2000
          })
        }
      })
    },
    consoleSet () { // 设置控制台访问形式
      this.isShowVmConsoleTypeDialog = true
    },
    getConsoleType (data) {
      this.isShowVmConsoleTypeDialog = false
      if (data.type) {
        this.consoleSettingType = data.type == '1' ? this.$t('vm.outNet') : data.type == '2' ? this.$t('vm.innerNet') : this.$t('system.noSetting')
      }
    },
    closeConsoleType () {
      this.isShowVmConsoleTypeDialog = false
    },
    autoLogin () { // 设置自动登录
      if (localStorage.getItem('autoLogin')) {
        this.$confirm(this.$t('setting.cancelAutoLogin'), this.$t('resource.tip')).then(() => {
          localStorage.removeItem('autoLogin')
          store.delete('autoLogin')
          this.isSettingAutoLogin = ''
          this.$message({
            message: this.$t('setting.cancelSuccess'),
            type: 'success',
            duration: 1500
          })
        })
      } else {
        this.$confirm(this.$t('setting.setAutoLogin'), this.$t('resource.tip')).then(() => {
          localStorage.setItem('autoLogin', true)
          store.set('autoLogin', true)
          this.isSettingAutoLogin = this.$t('setting.hasbeenAutoLogin')
          this.$message({
            message: this.$t('setting.settingSuccess'),
            type: 'success',
            duration: 1500
          })
        })
      }
    },
    submitResolution () {
      if (!this.resolution) {
        this.$message({
          message: this.$t('setting.selectResolution'),
          type: 'warning',
          duration: 1500
        })
        return false
      }
      this.$nprogress.start()
      this.$Api.system.setResolution({Current: this.resolution}).then(res => {
        this.$nprogress.done()
        if (res.Status.code == 0) {
          this.$message({
            message: this.$t('setting.settingSuccess'),
            type: 'success',
            duration: 1500
          })
        } else {
          this.$message({
            message: res.Status.messageCn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    ipTypeSetting (type) {
      this.$refs.net.clearValidate()
    },
    submitNet () {
      clearInterval(this.getAgentState)
      this.getAgentState = null
      // 如果没有修改
      let checkAfter = JSON.stringify(this.net)
      if (checkAfter == this.cur_netInfo && this.ipType == this.net.ipType) {
        this.$message({
          message: this.$t('setting.noEdit'),
          type: 'warning',
          duration: 1500
        })
        return false
      }
      let validFlag = 1
      if (this.ipType == 2) {
      // 需要校验现有内容是否正确
        this.$refs.net.validate((valid) => {
          if (!valid) {
            validFlag = 0
            return false
          }
        })
      }
      if (validFlag == 1) {
        this.confirmNetSetting()
      }
    },
    // 提交网络设置
    // 网络设置前
    confirmNetSetting () {
      this.$nprogress.start()
      let Dns = []
      Dns.push(this.net.fService, this.net.backupService)
      var params = {
        'Name': this.net.name,
        'Dhcp': this.ipType == 1,
        'Ip': this.net.Ip,
        'Netmask': this.net.Netmask,
        'Gateway': this.net.Gateway,
        'Macaddr': this.net.Macaddr,
        'Dns': Dns
      }
      this.$Api.system.hostMsg().then(res => {
        if (res.Uac) {
          this.submitNetSetting(params)
        } else { // 如果没有管理员权限，先要开启
          this.$message({
            message: this.$t('setting.agentAuthTip'),
            type: 'warning',
            duration: 5000
          })
          localStorage.setItem('noErrorTipFlag', true)
          this.$Api.system.runWithManager().then(res => {
          }).catch(() => {
            this.getAgentState = setInterval(() => {
              // 测试agent相关接口
              // 如果接口调通了，说明用户确定/取消了使用管理员权限，可以继续执行升级操作
              this.$Api.system.hostMsg().then(res => {
                localStorage.removeItem('noErrorTipFlag')
                clearInterval(this.getAgentState)
                this.getAgentState = null
                if (res.Uac) {
                  this.submitNetSetting(params)
                } else {
                  this.$message({
                    message: this.$t('setting.noAuth'),
                    type: 'warning',
                    duration: 1500
                  })
                }
              })
            }, 5000)
          })
        }
      }).catch(() => {
        this.$message({
          message: this.$t('setting.agentAuthTip'),
          type: 'warning',
          duration: 5000
        })
      })
    },
    submitNetSetting (params) {
      // eslint-disable-next-line standard/object-curly-even-spacing
      this.loadingHidden = this.$loading({ target: '.dialog-cont'})
      // 设置后可能存在接口无法调用net::ERR_ADDRESS_UNREACHABLE
      // 在设置成功前先进行些操作
      this.$Api.computers.closeAllVms({}).then(res => { // 一键杀死topview拉起的remote-view

      })
      this.$Api.system.netSeeting(params).then(res => {
        this.$nprogress.done()
        this.loadingHidden.close()
        if (res.Status.code == 0) {
          this.$message({
            message: this.$t('setting.settingSuccess'),
            type: 'success',
            duration: 1500
          })
          localStorage.setItem('netSettingFlag', true)
          this.getNetInfos()
        } else {
          this.$message({
            message: res.Status.messageCn,
            type: 'error',
            duration: 1500
          })
        }
      }).catch(() => {
        this.loadingHidden.close()
      })
    },
    submitNetTest () {
      this.$refs.netTest.validate((valid) => {
        if (!valid) {
          return false
        }
        this.netTest.PingText = ''
        let wbMsg = []
        let type = this.url && this.url.substr(0, 5) == 'https' ? 'wss' : 'ws'
        // let ip = type == 'wss' ? this.url.substr(8, this.url.length) : this.url.substr(7, this.url.length)
        let wsurl = `${type}://localhost:9888/ws/vdiagent/ping`
        this.netWebsocket = new WebSocket(wsurl)
        let params = {'Count': parseInt(this.netTest.Count),
          'Addr': this.netTest.Addr}
        this.netWebsocket.onopen = () => {
          this.netWebsocket.send(JSON.stringify(params))
        }
        this.netWebsocket.onmessage = (msg) => {
          wbMsg.push(msg.data)
          this.netTest.PingText = wbMsg.toString().replace(/,/g, '\r\n')
        }
        this.netWebsocket.onerror = (msg) => {
        }
        this.netWebsocket.onclose = (msg) => {
        }
      })
    },
    getNetInfos () {
      this.$Api.system.netInfos().then(res => {
        if (res.Status.code == 0) {
        // if (!res.Dhcp) {
          let result = res
          if (result.NetInterfaceList) {
            this.netInfoRes = result.NetInterfaceList
            this.cur_netcard = this.netInfoRes[0].Name
            result = this.netInfoRes[0]
          }
          this.ipType = result.Dhcp ? '1' : '2'
          this.net.ipType = result.Dhcp ? '1' : '2'
          this.net.name = result.Name
          this.net.Ip = result.Ip
          this.net.Netmask = result.Netmask
          this.net.Gateway = result.Gateway
          this.net.Macaddr = result.Macaddr
          this.net.fService = result.Dns && result.Dns[0]
          this.net.backupService = result.Dns && result.Dns[1] ? result.Dns[1] : ''
          this.cur_netInfo = JSON.stringify(this.net)
          setTimeout(() => {
            this.$refs.net.clearValidate()
          })
        } else {
          this.$message({
            message: res.Status.messageCn,
            type: 'warning',
            duration: 3000
          })
        }
      })
    },
    changeNetcard () {
      let filter = this.netInfoRes.filter(item => {
        return item.Name == this.cur_netcard
      })
      let filterVlue = filter[0]
      this.net = {
        ip: filterVlue.Dhcp ? '1' : '2',
        name: filterVlue.Name,
        Ip: filterVlue.Ip,
        Netmask: filterVlue.Netmask,
        Gateway: filterVlue.Gateway,
        Macaddr: filterVlue.Macaddr,
        fService: filterVlue.Dns && filterVlue.Dns[0],
        backupService: filterVlue.Dns && filterVlue.Dns[1] ? filterVlue.Dns[1] : ''
      }
      this.ipType = filterVlue.Dhcp ? '1' : '2'
      this.cur_netInfo = JSON.stringify(this.net)
      setTimeout(() => {
        this.$refs.net.clearValidate()
      })
    },
    getReso () {
      this.$Api.system.resolutionInfo().then(res => {
        if (res.Available) {
          this.resolutionList = res.Available
          this.resolution = res.Current
        } else {
          this.$message({
            message: res.Status.code,
            type: 'warning',
            duration: 3000
          })
        }
      }).catch(() => {
        this.$message({
          message: this.$t('login.agentTip'),
          type: 'warning',
          duration: 3000
        })
      })
    },
    closeHost () { // 关机
      this.$confirm(this.$t('setting.sureShutdown'), this.$t('resource.tip')).then(() => {
        this.$Api.system.agentPoweroff({Delay: 0}).then(res => {
        })
        this.closeSocket()
      })
    },
    restartHost () {
      this.$confirm(this.$t('setting.sureReboot'), this.$t('resource.tip')).then(() => {
        this.$Api.system.agentReboot({Delay: 0}).then(res => {
        })
        this.closeSocket()
      })
    },
    useManagerRole () { // 权限拓展，agent开启管理员权限
      localStorage.setItem('noErrorTipFlag', true)
      this.$Api.system.hostMsg().then(res => {
        localStorage.removeItem('noErrorTipFlag')
        if (res.Status.code == 0) {
          // 存主机信息
          this.setHostInfos(res)
          sessionStorage.setItem('hostId', res.Name)
          if (res.Uac) {
            this.$message({
              message: this.$t('setting.agentHasAdministrator'),
              type: 'warning',
              duration: 3000
            })
          } else {
            this.$confirm(this.$t('setting.useManagerTip'), this.$t('resource.tip')).then(() => {
              localStorage.setItem('useManagerRoleFlag', true)
              this.$Api.system.runWithManager().then(res => {
              })
            })
          }
        } else {
          this.$message({
            message: res.Status.messageCn,
            type: 'warning',
            duration: 3000
          })
        }
      })
        .catch(() => {
          localStorage.removeItem('noErrorTipFlag')
          this.$message({
            message: this.$t('login.agentTip'),
            type: 'warning',
            duration: 3000
          })
        })
    }
  },
  watch: {
    visible (val) {
    }
  },
  mounted () {
    localStorage.removeItem('netSettingFlag') // 防止在登录页点开设置
    if (localStorage.getItem('autoLogin')) {
      this.isSettingAutoLogin = this.$t('setting.hasbeenAutoLogin')
    } else {
      this.isSettingAutoLogin = ''
    }
    this.getNetInfos()
    if (!os.platform().includes('win')) {
      this.isWin = false
    }
    if (localStorage.getItem('consoleType') || store.get('consoleType')) {
      let type = localStorage.getItem('consoleType') || store.get('consoleType')
      this.consoleSettingType = type == '1' ? this.$t('vm.outNet') : type == '2' ? this.$t('vm.innerNet') : this.$t('system.noSetting')
    } else {
      this.consoleSettingType = this.$t('system.noSetting')
    }
    // this.getReso()
  },
  beforeDestroy () {
    clearInterval(this.getAgentState)
    this.getAgentState = null
    localStorage.removeItem('noErrorTipFlag')
  }
}
</script>
<style lang="scss" scoped>
.el-radio-group{
  margin-bottom: 13px !important;
  .el-radio{
    width: 100%;
    padding: 10px 0px;
  }
}
.dialog-cont{
  height:550px;
  // tab
  /deep/.el-radio-button__inner{
    // width: 125px;
    border-radius:0px !important;
  }
  .input-cont{
    height: 400px;
    padding-left: 10px;
  }
  .net-cont{
    /deep/.el-form-item{
      margin-left: 30px;
    }
    /deep/.el-form-item__content{
      width:312px;
    }
    /deep/.el-form-item__label::before{
      display: none;

    }
  }
}
.netTest-left-label{
  /deep/.el-form-item__label{
    width: 101px;
  }
  /deep/.el-form-item__content{
    display: inline-block;
  }
}
.netTest-right-label{
  margin-left: 5px;
  /deep/.el-form-item__label{
    width: 75px;    text-align: center;
  }
  /deep/.el-form-item__content{
    display: inline-block;
  }

}
.el-form-item{
  margin-bottom:20px;
}
// .el-input{
//   width: 310px;
// }
.net-button-label{
  margin-top: 40px;
  padding-top: 20px;
  height: 50px;
  border-top: 1px solid #E7E7E7;
  div{
    position: absolute;
    right: 20px;
  }
}
.netTest-button-label{
  margin-top: 287px;
  padding-top: 20px;
  height: 50px;
  border-top: 1px solid #E7E7E7;
  div{
    position: absolute;
    right: 20px;
  }
}
.host-opt-cont{
    margin-top: 15px;
    margin-left: 19px;
  div{
    margin-top: 18px;
    // display: flex;
    margin-left: 65px;
    .el-button{
          border-color: #DCDFE6;
    color: #666;min-width: 98px;
    }
    span{
      display: inline-block;
      height: 32px;
      line-height: 32px;
      margin-left: 18px;
      padding-left: 18px;
      border-left: 1px dashed #DCDFE6;
    }
  }
  .tip{
    padding-left: 200px;
    margin-top: 3px;color: #888;
  }
}
.host-opt-cont-en{
    margin-top: 15px;
    margin-left: -22px;
  div{
    margin-top: 18px;
    display: flex;
    margin-left: 107px;
    .el-button{
          border-color: #DCDFE6;
    color: #666;    width: 130px;
    }
    span{
      display: inline-block;   width: 380px;
      word-break: break-word;
      height: 32px;
      line-height: 32px;
      margin-left: 18px;
      padding-left: 18px;
      border-left: 1px dashed #DCDFE6;
    }
  }
  .tip{
    padding-left: 274px;margin-top: 18px;color: #888;
  }
}
.console-opt-cont-en{
    margin-top: 15px;
  div{
    margin-top: 18px;
    display: flex;
    margin-left: 107px;
    .el-button{
          border-color: #DCDFE6;
    color: #666;    width: 130px;
    }
    span{
      display: inline-block;   width: 380px;
      word-break: break-word;
      line-height: 20px;
      margin-left: 18px;
      padding-left: 18px;
      border-left: 1px dashed #DCDFE6;
    }
  }
  .tip{
    padding-left: 270px;color: #888;
  }
}
.el-button{
	min-width: 72px;
	height: 32px;
	border-radius: 0px;
}
</style>

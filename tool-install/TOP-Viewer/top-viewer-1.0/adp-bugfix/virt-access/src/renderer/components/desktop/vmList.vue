<template>
  <div>
    <el-checkbox-group
      v-model="checkedVmLists"
      class="vm-checkBox"
      @change="selectVms">
      <div
        v-if="!showBlockPic">
        <div
          v-for="(item,index) in computerList"
          :key="index"
          class="vm-cont">
          <div
            :class="(cur_vm.uuid==item.uuid?true:false||checkedVmLists.some(el=>el.uuid==item.uuid))?'vm-pic-checked vm-pic':'vm-pic'"
            @mouseover="showVmOpt(item)"
            @mouseleave="leaveVm">
            <el-checkbox
              v-show="(cur_vm.uuid==item.uuid?true:false||checkedVmLists.some(el=>el.uuid==item.uuid))&&item.action=='noaction'"
              :key="index"
              class="select-vm"
              :label="item">
              .
            </el-checkbox>
            <div
              v-show="cur_vm.uuid==item.uuid?true:false||checkedVmLists.some(el=>el.uuid==item.uuid)"
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
              v-html="$t('cloud.vm_state')+getVmState(item.state)+' | '+$t('cloud.act')+item.flagtxt" />
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
  </div>
</template>
<script>
export default {
  props: ['computerList', 'showBlockPic'],
  data () {
    return {
      cur_vm: '',
      checkedVmLists: []

    }
  },
  methods: {
    showVmOpt (vm) {
      this.cur_vm = vm
      this.$emit('showVmOpt', vm)
    },
    leaveVm () {
      this.cur_vm = ''
      this.$emit('leaveVm')
    },
    selectVms (value) {
      this.$emit('selectVms', value, this.checkedVmLists)
    },
    computerOpt (operate, vm) {
      this.$emit('computerOpt', operate, vm)
    },
    checkConsoleState (val) {
      this.$emit('checkConsoleState', val)
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
    }

  }
}
</script>>
<style lang="scss" scoped>

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

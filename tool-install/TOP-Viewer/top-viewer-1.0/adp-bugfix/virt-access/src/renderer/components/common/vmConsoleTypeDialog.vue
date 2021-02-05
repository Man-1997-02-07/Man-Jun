<template>
  <div>
    <el-dialog
      :title="type_dialog_val=='systemSetting'?`${$t('system.setting')}`:`${$t('resource.tip')}`"
      :visible.sync="visible"
      :close-on-click-modal="false"
      :modal-append-to-body="false"
      width="450px"
      append-to-body
      class="vm-opt-dialog"
      @close="closeDialog">
      <div style="height: 40px;padding-top:30px;">
        <el-radio
          v-model="type"
          label="1">
          {{ $t('vm.outNet') }}
        </el-radio>
        <el-radio
          v-model="type"
          label="2">
          {{ $t('vm.innerNet') }}
        </el-radio>
        <el-radio
          v-if="type_dialog_val!=='systemSetting'"
          v-model="type"
          label="3">
          {{ $t('resource.cancelExecute') }}
        </el-radio>
        <el-radio
          v-if="type_dialog_val=='systemSetting'"
          v-model="type"
          label="3">
          {{ $t('system.clearConsoleSetting') }}
        </el-radio>
      </div>
      <div
        v-if="type_dialog_val!=='systemSetting'"
        style="margin-botton:20px;color:#999;">
        {{ $t('system.consoleTypeTip') }}
      </div>
      <div class="btn-cont">
        <el-button
          id="confirm_opt_vm_console"
          type="primary"
          @click="confirmType()">
          {{ $t('resource.confirm') }}
        </el-button>
        <el-button @click="closeDialog">
          {{ $t('resource.close') }}
        </el-button>
      </div>
    </el-dialog>
  </div>
</template>
<script>
import ElectronStore from 'electron-store'
var electronStore = new ElectronStore() // 存储数据
var os = require('os')
const exec = require('child_process').exec
export default {
  props: ['isShowVmConsoleTypeDialog', 'type_dialog_val'],
  data () {
    return {
      type: '1',
      visible: false
    }
  },
  watch: {
    isShowVmConsoleTypeDialog (val) {
      this.visible = val
    }
  },
  mounted () {
    this.visible = this.isShowVmConsoleTypeDialog
    if (this.type_dialog_val == 'systemSetting') {
      // 如果内容是从高级设置打开的
      // 需要读取获取设置信息
      if (os.platform().includes('win')) {
        // 如果是windows系统
        this.type = localStorage.getItem('consoleType')
      } else {
        this.type = electronStore.get('consoleType')
      }
    }
  },
  methods: {
    confirmType () { // 确定选中的类型
      if (this.type_dialog_val == 'systemSetting') {
        if (this.type == 3) {
          // 如果选择重置
          electronStore.delete('consoleType')
          localStorage.removeItem('consoleType')
        } else {
          electronStore.set('consoleType', this.type)
          localStorage.setItem('consoleType', this.type)
        }
      }
      this.$emit('getConsoleType', {type: this.type, data: this.type_dialog_val})
    },
    closeDialog () {
      this.$emit('closeConsoleType')
    }

  }
}
</script>
<style lang="scss" scoped>
/deep/.el-dialog__body{
  height:145px !important;
}
.btn-cont{
    position: absolute;
    right: 10px;    bottom: 10px;

}
</style>

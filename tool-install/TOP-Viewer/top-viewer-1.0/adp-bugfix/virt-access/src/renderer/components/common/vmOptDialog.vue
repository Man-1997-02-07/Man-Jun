<template>
  <div>
    <el-dialog
      :title="`${$t('resource.tip')}`"
      :visible.sync="visible"
      :close-on-click-modal="false"
      :modal-append-to-body="false"
      width="500px"
      class="vm-opt-dialog"
      @close="closeOptDialog">
      <div style="height: 110px;">
        <div class="notice-content">
          <i class="sg-iconfont im-icon-gaojing" />
          {{ tipContent }}
        </div>
        <div>
          <el-checkbox
            v-if="isShowBtn"
            id="compute_dialogOpt_safety"
            v-model="safe_flag"
            :disabled="vms_info.action === 'migrating' || vms_info.action === 'pending'"
            style="float: left;position: relative;top:8px;">
            {{ (opt_type !== 'shutoff' && opt_type !== 'bath_shutoff') ? $t('resource.safeRestart') : ((opt_type == 'shutoff' || opt_type == 'bath_shutoff') && vms_info.ga_state!=='disconnected') ? $t('resource.safeClose'):'' }}
          </el-checkbox>
        </div>
        <div class="btn-cont">
          <el-button
            id="confirm_opt_vm"
            class="primary-btn"
            @click="submitOpt(opt_type)">
            {{ $t('resource.confirm') }}
          </el-button>
          <el-button @click="closeOptDialog">
            {{ $t('resource.close') }}
          </el-button>
        </div>
      </div>
    </el-dialog>
  </div>
</template>
<script>
export default {
  props: ['refreshList', 'desktopFlag', 'opt_type', 'vm_opt_params', 'tipContent', 'vms_info', 'isShowVmOptDialog'],
  data () {
    return {
      visible: this.isShowVmOptDialog,
      safe_flag: true,
      isShowBtn: false
    }
  },
  methods: {
    closeOptDialog () {
      this.$emit('closeOptDialog')
    },
    submitOpt (type) {
      let res_Api = ''
      switch (type) {
        case 'shutoff':
          res_Api = this.isShowBtn && this.safe_flag ? this.$Api.computers.vmSafeShutoff : this.$Api.computers.vmShutoff
          break
        case 'reboot':
          res_Api = this.isShowBtn && this.safe_flag ? this.$Api.computers.vmSafeReboot : this.$Api.computers.vmReboot
          break
        case 'start':
          res_Api = this.desktopFlag ? this.$Api.desktop.desktopStartVm : this.$Api.computers.vmStart
          break
        case 'restore':
          res_Api = this.$Api.computers.vmRestore
          break
        case 'resume':
          res_Api = this.$Api.computers.vmResume
          break
        case 'bath_shutoff':
          res_Api = this.isShowBtn && this.safe_flag ? this.$Api.computers.vmSafeBatchShutoff : this.$Api.computers.vmBatchShutoff
          break
        case 'bath_reboot':
          res_Api = this.isShowBtn && this.safe_flag ? this.$Api.computers.vmSafeBatchReboot : this.$Api.computers.vmBatchReboot
          break
        case 'bath_start':
          res_Api = this.desktopFlag ? this.$Api.desktop.desktopBatchStartVm : this.$Api.computers.vmBatchStart
          break
        case 'bath_restore':
          res_Api = this.$Api.computers.vmBatchRestore
          break
        case 'bath_resume':
          res_Api = this.$Api.computers.vmBatchResume
          break
      }
      // eslint-disable-next-line standard/object-curly-even-spacing
      let loading = this.$loading({ target: '.vm-opt-dialog'})
      res_Api(this.vm_opt_params).then(res => {
        loading.close()
        this.dealRequest(res, type)
      }).catch((error) => {
        loading.close()
        // 兼容旧接口，在新接口404的清空时调用
        if (error.response.status == 404 && this.desktopFlag && type == 'start') {
          this.dealVmStartApi()
        }
      })
    },
    dealVmStartApi () {
      this.$Api.desktop.desktopBatchStartVm(this.vm_opt_params).then(res => {
        this.dealRequest(res, 'start')
      })
    },
    dealRequest (res, type) {
      if (res.scode == 0) {
        if (type.includes('bath')) {
          const data = res.data
          this.bath_info(type, data)
        } else {
          if (type == 'start' && res.data.fail > 0) {
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
          }
        }
        this.closeOptDialog()
      } else {
        this.$message({
          message: res.message_cn,
          type: 'error',
          duration: 1500
        })
      }
    },
    bath_info (type, data) {
      const typeName = type == 'bath_reboot' ? '重启' : (type == 'bath_shutoff' ? '关闭' : '启动')
      var vms = []
      vms = this.vms_info
      var tipname = this.desktopFlag ? '云桌面' : '云服务器'
      if (data.fail > 0) {
        const h = this.$createElement
        const error_info = `${typeName}${vms.length}台${tipname}，其中${data.fail}台${typeName}失败，具体如下：`
        const error_en_info = `${type} ${vms.length} cloud servers, ${data.fail} of them failed to ${type}：`
        let err_ms = ''
        let err_en_ms = ''
        const show_err_ms = []
        const show_err_en_ms = []
        data.results.forEach(item => {
          if (item.message_cn !== '成功') {
            vms.forEach(key => {
              if (item.data.compute_uuid == key.uuid) {
                err_ms = `${tipname}${key.name}${typeName}失败，失败原因：${item.message_cn}`
                show_err_ms.push(h('p', null, [
                  h('p', null, err_ms)
                ]))
                err_en_ms = `The ${key.name} failed to ${type}. The reason for the failure is ${item.message}`
                show_err_en_ms.push(h('p', null, [
                  h('p', null, err_en_ms)
                ]))
              }
            })
          }
        })
        this.$message({
          message: h('p', null, [
            h('p', null, this.language == 'zh' ? error_info : error_en_info),
            h('p', null, this.language == 'zh' ? show_err_ms : show_err_en_ms)
          ]),
          type: 'error',
          duration: 4500
        })
      } else {
        this.$message({
          message: this.language == 'zh' ? `${data.success ? data.success : vms.length}台${tipname}${typeName}成功!` : `${data.success} cloud services operation successful`,
          type: 'success',
          duration: 4500
        })
      }
    }
  },
  mounted () {
    if (this.opt_type.includes('bath')) {
      var flag = true
      // 如果是批量操作，遍历数据，看是否能够安全重启/关机
      for (const item of this.vms_info) {
        if (item.ga_state == 'disconnected') {
          flag = false // agent是连接状态才支持
          break
        }
      }
    }
    this.isShowBtn = Boolean((this.opt_type == 'shutoff' && this.vms_info.ga_state !== 'disconnected') || (this.opt_type == 'reboot' && this.vms_info.ga_state !== 'disconnected') || (this.opt_type == 'bath_shutoff' && flag) || (this.opt_type == 'bath_reboot' && flag))
  }

}
</script>
<style lang="scss" scoped>
.notice-content{
      height: 80px;
      i{
        color: #E6A23C; padding-right:5px ;
      }
}
.primary-btn{
  background-color: #3583e3;
  color: white;
}
.btn-cont{
    position: absolute;
    right: 10px;

}
</style>

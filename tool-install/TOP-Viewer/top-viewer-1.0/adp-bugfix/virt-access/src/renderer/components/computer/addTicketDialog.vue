<template>
  <div>
    <el-dialog
      :title="`${$t('ticket.addTicket')}`"
      :visible.sync="showDialog"
      :close-on-click-modal="false"
      :modal-append-to-body="false"
      width="580px"
      @close="closeTicketDialog">
      <el-form
        ref="addForm"
        :rules="rules"
        :model="addForm"
        :label-width="language=='zh'?'120px':'156px'">
        <el-form-item
          :label="`${$t('ticket.type')}`">
          <el-select
            v-model="cur_ticketType"
            @change="changeTicketType">
            <el-option
              v-for="(item, index) in ticketTypeList"
              :key="index"
              :label="item.type"
              :value="item.typeId" />
          </el-select>
        </el-form-item>
        <el-form-item
          :label="`${$t('ticket.ticketName')}`"
          prop="ticketName">
          <el-input
            id="add_ticketoptDia_ticketName"
            v-model="addForm.ticketName"
            type="text"
            :placeholder="`${$t('ticket.nameTip')}`" />
        </el-form-item>
        <el-form-item
          :label="`${$t('ticket.reason')}`"
          prop="reason">
          <el-input
            id="add_ticketoptDia_reason"
            v-model="addForm.reason"
            type="textarea"
            maxlength="100"
            show-word-limit
            :placeholder="`${$t('ticket.reasonTip')}`" />
        </el-form-item>
        <el-form-item
          :label="`${$t('ticket.process')}`"
          prop="process">
          <el-select
            id="add_ticketoptDia_process"
            v-model="addForm.process">
            <el-option
              v-for="(item, index) in processes"
              :key="index"
              :label="item.process_name"
              :value="item.process_uuid" />
          </el-select>
        </el-form-item>
        <el-form-item
          :label="`${$t('ticket.vm')}`"
          prop="compute">
          <el-select
            id="add_ticketoptDia_vmChoose"
            v-model="addForm.compute"
            @change="changeVm(cur_ticketType,addForm.compute)">
            <el-option
              v-for="item in vmsList"
              :key="item.index"
              :label="item.name"
              :value="item.uuid" />
          </el-select>
        </el-form-item>
        <!-- 虚拟机cpu修改 -->
        <div v-if="cur_ticketType=='cpu'">
          <el-form-item
            :class="vm_cpu_error?'cpu-num is-error':'cpu-num'"
            :label="`${$t('ticket.cpu')}`"
            prop="cpu_count">
            <el-input
              id="function_apply_optDia_cpuCount"
              v-model="addForm.cpu_count"
              type="text"
              @input="check_setVm_cpu(addForm.cpu_count)" />
            <!-- placeholder="请输入CPU核数" -->
            <span
              class="apply-error-info"
              style="display: flex;top: 26px;">{{
                vm_cpu_error
              }}</span>
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.socket')}`"
            prop="cpu_sockets">
            <el-input
              id="function_apply_optDia_cpuSoceket"
              v-model="addForm.cpu_sockets"
              type="text" />
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.cores')}`"
            prop="cpu_socket_count">
            <el-input
              id="function_apply_optDia_cpuSocketCount"
              v-model="addForm.cpu_socket_count"
              type="text" />
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.cores')}`"
            prop="Max_cpu">
            <el-input
              id="function_apply_optDia_cpuMax"
              v-model="addForm.Max_cpu"
              type="text"
              :placeholder="`${$t('ticket.maxCpuTip')}`"
              disabled />
          </el-form-item>
        </div>

        <div v-if="cur_ticketType=='memory'">
          <el-form-item
            :label="`${$t('ticket.memory')}`"
            prop="vm_memory_size"
            class="memory-label">
            <el-input
              id="function_apply_optDia_mem"
              v-model="addForm.vm_memory_size"
              type="text"
              :placeholder="`${$t('ticket.memoryTip')}`"
              :disabled="!Boolean(addForm.compute)">
              <el-select
                id="function_apply_optDia_memUnit"
                slot="append"
                v-model="addForm.vm_memory_unit">
                <el-option
                  label="T"
                  value="T" />
                <el-option
                  label="G"
                  value="G" />
              </el-select>
            </el-input>
          </el-form-item>
        </div>

        <div v-if="cur_ticketType=='mountDisk'">
          <el-form-item
            :label="`${$t('ticket.driverType')}`"
            prop="disk_type">
            <el-radio-group
              id="function_apply_optDia_diskType"
              v-model="addForm.disk_type">
              <el-radio
                id="function_apply_optDia_ide"
                label="ide">
                ide
              </el-radio>
              <el-radio
                id="function_apply_optDia_sata"
                label="sata">
                sata
              </el-radio>
              <el-radio
                id="function_apply_optDia_virtio"
                label="virtio">
                virtio
              </el-radio>
              <el-radio
                id="function_apply_optDia_scsi"
                label="scsi">
                scsi
              </el-radio>
            </el-radio-group>
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.volumeType')}`"
            prop="volume_type">
            <el-radio-group
              id="function_apply_optDia_volumeType"
              v-model="addForm.volume_type"
              @change="changeVolumeType(addForm.volume_type)">
              <el-radio
                id="function_apply_optDia_innerVolume"
                label="inner"
                :disabled="isedit">
                {{ $t('ticket.inV') }}
              </el-radio>
              <!---外置卷-->
              <el-radio
                id="function_apply_optDia_outsideVolume"
                label="out"
                :disabled="isedit">
                {{ $t('ticket.outV') }}
              </el-radio>
            </el-radio-group>
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.volume')}`"
            prop="volume">
            <el-select
              id="function_apply_optDia_volumeSelect"
              v-model="addForm.volume"
              @visible-change="beforeSelectVolume">
              <el-option
                v-for="item in selectVolumeList"
                :key="item.index"
                :label="item.name"
                :value="item.uuid" />
            </el-select>
          </el-form-item>
        </div>

        <div v-if="cur_ticketType=='addDisk'">
          <el-form-item
            :label="`${$t('ticket.specification_volume')}`"
            prop="specification_volume">
            <el-select
              id="function_apply_optDia_addDiskSpec"
              v-model="addForm.specification_volume"
              @change="selectedSpecificationVolume(addForm.specification_volume)"
              @visible-change="beforeSpecificationVolume">
              <el-option
                v-for="item in selectVolumeList"
                :key="item.index"
                :label="item.name"
                :value="item.uuid" />
            </el-select>
          </el-form-item>
          <el-form-item
            v-if="showPoolsSelect"
            :label="`${$t('ticket.pool')}`"
            prop="specification_pool">
            <el-select
              id="function_apply_optDia_Specilication_pool"
              v-model="addForm.specification_pool">
              <el-option
                v-for="(item,index) in internalPools"
                :key="index"
                :label="item.name"
                :value="item.uuid" />
            </el-select>
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.driverType')}`"
            prop="disk_type"
            class="add-disk">
            <el-radio-group
              id="function_apply_optDia_addDiskType"
              v-model="addForm.disk_type">
              <el-radio
                id="function_apply_optDia_ide_not"
                label="ide">
                ide
              </el-radio>
              <el-radio
                id="function_apply_optDia_sata"
                label="sata">
                sata
              </el-radio>
              <el-radio
                id="function_apply_optDia_virtio_not"
                label="virtio">
                virtio
              </el-radio>
              <el-radio
                id="function_apply_optDia_scsi_not"
                label="scsi">
                scsi
              </el-radio>
            </el-radio-group>
          </el-form-item>
        </div>

        <div v-if="cur_ticketType=='addNet'">
          <el-form-item
            :label="`${$t('ticket.netcardType')}`"
            prop="netcard_type">
            <el-radio-group
              id="function_apply_optDia_netType"
              v-model="addForm.netcard_type">
              <el-radio
                id="function_apply_optDia_virtio"
                label="virtio">
                virtio
              </el-radio>
              <el-radio
                id="function_apply_optDia_e1000"
                label="e1000">
                e1000
              </el-radio>
              <el-radio
                id="function_apply_optDia_rtl8139"
                label="rtl8139">
                rtl8139
              </el-radio>
            </el-radio-group>
          </el-form-item>
          <el-form-item
            :label="`${$t('ticket.netType')}`"
            prop="internet_type">
            <el-radio-group
              id="function_apply_optDia_net"
              v-model="addForm.internet_type">
              <el-radio
                id="function_apply_optDia_internal"
                label="internal">
                {{ $t('ticket.vpc') }}
              </el-radio>
              <el-radio
                id="function_apply_optDia_local"
                label="local">
                {{ $t('ticket.local') }}
              </el-radio>
            </el-radio-group>
          </el-form-item>
          <el-form-item
            v-if="istenant && addForm.internet_type === 'internal'"
            :label="`${$t('ticket.switch')}`"
            prop="switch">
            <el-select
              id="function_apply_optDia_netSwitch"
              v-model="addForm.switch"
              :placeholder="`${$t('ticket.switchTip')}`"
              @visible-change="beforeSelectSwitch">
              <el-option
                v-for="(item, index) in selectSwitchList"
                :key="index"
                :label="item.name"
                :value="item.uuid" />
            </el-select>
          </el-form-item>
        </div>

        <div class="ticket-button">
          <el-button
            id="confirm_add_ticket"
            type="primary"
            @click="addTicket(cur_ticketType)">
            {{ $t('resource.confirm') }}
          </el-button>
          <el-button @click="closeTicketDialog">
            {{ $t('resource.close') }}
          </el-button>
        </div>
        <div style="clear:both;" />
      </el-form>
    </el-dialog>
  </div>
</template>
<script>
import Basic from '@/test/basic.js'
import ticketjs from './ticket.js'
import { handleShareVolumeListData } from '@/components/common/js/dataHandle.js'
export default {
  mixins: [Basic, ticketjs],
  props: ['showAddTicketDialog', 'computerList'],
  data () {
    return {
      showDialog: this.showAddTicketDialog,
      processes: [],
      vmsList: this.computerList, // 虚拟机列表
      selectVolumeList: [],
      internalPools: [],
      selectSwitchList: [],
      switchsList: [],
      volSpecifications: [],
      shareVolumeList: [],
      vm_cpu_error: '',
      isedit: false,
      showPoolsSelect: false, // 展示存储池的选择
      ticketTypeList: [{type: this.$t('ticket.changeCpu'), typeId: 'cpu'}, {type: this.$t('ticket.changeMemory'), typeId: 'memory'}, {type: this.$t('ticket.mountDisk'), typeId: 'mountDisk'},
        {type: this.$t('ticket.addDisk'), typeId: 'addDisk'}, {type: this.$t('ticket.addNet'), typeId: 'addNet'}],
      cur_ticketType: 'cpu',
      addForm: {
        createCount: 1,
        ticketName: '',
        reason: '',
        vm_name: '',
        groupPath: [],
        specification_vm: '',
        template_vm: '',
        volume_name: '',
        specification_volume: '',
        specification_pool: '', // 当共享规格时 需要制定池的资源
        compute: '',
        volume: '',
        cpu_count: '',
        cpu_sockets: '',
        cpu_socket_count: '',
        Max_cpu: '',
        vm_memory_size: '',
        vm_memory_unit: 'G',
        volume_memery_Size: '',
        volume_memery_unit: 'G',
        volume_type: 'inner',
        netcard_type: 'virtio', // 网卡
        internet_type: 'internal', // 网络类型
        switch: '',
        // localNet:'',
        secureVM: '', // 安全虚拟机
        vip_count: '',
        gateway_name: '', // 网关名称
        cpuSize: 50,
        memerySize: 100,
        storageSize: 1024,
        screen_shots: 10,
        memery_snap: 10,
        zone_uuid: '',
        switch_count: 1,
        router_count: 1,
        gateway_count: 1,
        disk_type: 'ide',
        secondNetworkList: [],
        process: ''
      },
      vm: {
        Max_Capacity: '', Capacity: ''
      },
      rules: {
        // 名称
        ticketName: [
          { required: true, validator: this.testName(this.$t('ticket.ticketName')), trigger: 'change' }],
        compute: [
          { required: true, message: this.$t('ticket.vmCannotEmpty'), trigger: 'change' }],
        process: [
          { required: true, message: this.$t('ticket.processCannotEmpty'), trigger: 'change' }],
        cpu_sockets: [
          { required: true, validator: this.check_setVm_cpu_sockets(this.$t('ticket.socket')), trigger: 'change' } ],
        cpu_socket_count: [
          { required: true, validator: this.check_setVm_cpu_socket_count(this.$t('ticket.cores')), trigger: 'change' }],
        // 虚拟机容量
        vm_memory_size: [
          {required: true, validator: this.checkVmMemSize(this.$t('ticket.memory')), trigger: 'change'}],
        // 卷uuid
        volume: [
          { required: true, message: this.$t('ticket.volumeCannotEmpty'), trigger: 'change' }],
        specification_volume: [
          { required: true, message: this.$t('ticket.specification_volumeCannotEmpty'), trigger: 'change' }],
        specification_pool: [
          { required: true, message: this.$t('ticket.poolCannotEmpty'), trigger: 'change' }]
      }
    }
  },
  methods: {
    closeTicketDialog () {
      this.$emit('closeAddTicketDialog', false)
    },
    changeTicketType (type) {
      this.$nextTick(() => {
        this.$refs['addForm'].clearValidate()
      })
      switch (type) {
        case 'mountDisk': // 挂载磁盘
          this.getvolumeListMethod(true)
          this.getSharaVolumeList()
          this.addForm.disk_type = 'ide'
          this.addForm.volume_type = 'inner'
          break
        case 'addDisk':
          this.getVolSpecificationList({
            pagenum: 0,
            pagesize: 0,
            cluster_uuid: this.cluster_uuid,
            filter_volume_device_type: 'target',
            noMultiple: true
          })
          this.addForm.disk_type = 'ide'
          break
        case 'addNet':
          // // 获取所有二层网络
          this.istenant && this.getSwitchList()
          break
      }
    },
    // 挂载磁盘，选择类型
    changeVolumeType (type) {
      this.addForm.volume = ''
      if (type == 'inner') { // 选内置
        this.getVolumeList()
      } else {
        this.selectVolumeList = this.shareVolumeList
        console.log('触发', this.selectVolumeList)
      }
    },
    // 获取外置卷
    getSharaVolumeList () {
      const params = {
        cluster_uuid: this.cluster_uuid, tenant: this.cur_tenant, pool_uuid: ''
      }
      this.$Api.computers.getShareVolumeList(params).then(res => {
        if (res.scode == 0) {
          this.shareVolumeList = handleShareVolumeListData(res.data)
          console.log('获取结果', this.shareVolumeList)
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500

          })
        }
      })
    },
    getvolumeListMethod (options) {
      const params = {
        cluster_uuid: this.cluster_uuid,
        tenant: this.cur_tenant,
        page_num: 0,
        page_size: 0,
        no_detail: true
      }
      if (options) {
      //   filter_mount_type  1：过滤未挂载的    2：过滤已经挂载的
        params.filter_mount_type = 2
        params.filter_user_uuid = this.isuser ? sessionStorage.getItem('useruuid') : null // 这个是为了区分租户-和这个租户下用户获得的资源
        params.filter_unsed_type = 3
        params.filter_dev_type = 'target@block'
      } else {
        params.filter_mount_type = 2
      }
      this.$Api.computers.volumeInternalList(params).then((response) => {
        if (response.scode == 0) {
          const data = response.data
          this.volumeList = []
          if (data == '' || !data) { return }
          if (data.List == null) { return }
          data.List.forEach((item, idx) => {
            item.name = item.Name
            item.uuid = item.UUID
          })
          this.volumeList = data.List
        }
      })
    },
    // 卷规格
    // 获取存储规格列表
    getVolSpecificationList (options) {
      const params = {
        tenant: this.cur_tenant,
        page_num: 0,
        page_size: 0,
        include_share: true
      }
      if (options) {
        params.filter_volume_device_type = options.filter_volume_device_type// 可以选择过滤卷类型 例：target
        params.cluster_uuid = options.cluster_uuid ? options.cluster_uuid : this.cluster_uuid
      } else {
        // 获取以及资源加参数：multiple: true
        // 仅列表调用时添加
        this.cur_tenant === 'default' && (params.multiple = true)
      }
      this.volSpecifications = []
      this.$Api.computers.specificationVolumeList(params).then((response) => {
        if (response.scode == 0) {
          const data = response.data
          this.volSpecifications = data.list
        } else {
          this.$message({
            message: response.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    // 交换机列表
    getSwitchList () {
      const params = {
        tenant: this.cur_tenant,
        page_num: 0,
        page_size: 0,
        cluster_uuid: this.cluster_uuid
      }
      if (this.cur_tenant === 'default') {
        params['multiple'] = true
      }
      this.$Api.computers.switchList(params).then(res => {
        if (res.scode === 0) {
          this.switchsList = res.data.switch_infos
        }
      })
    },
    // 如果选择的是共享规格，需要再选择池uuid
    selectedSpecificationVolume (val) {
      this.showPoolsSelect = false
      this.selectVolumeList.forEach(item => {
        if (item.uuid == val && item.is_share == true) {
          this.showPoolsSelect = true
          this.getPools()
        }
      })
    },
    getPools () {
      const params = {
        cluster_uuid: this.cluster_uuid,
        tenant: this.cur_tenant
      }
      this.internalPools = []
      this.$Api.computers.poolInternalList(parmas).then(res => {
        if (res.scode == 0) {
          this.internalPools = res.data
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    // 添加磁盘
    beforeSpecificationVolume (flag) {
      if (flag) {
        this.getAddVolumeList()
      }
    },
    getAddVolumeList () {
      let computerZone = ''
      this.selectVolumeList = []
      if (!this.addForm.compute) {
        this.$message({
          message: this.$t('ticket.vmCannotEmpty'),
          type: 'error',
          duration: 1500
        })
        return false
      } else {
        for (let i = 0; i < this.vmsList.length; i++) {
          if (this.vmsList[i].uuid == this.addForm.compute) {
            computerZone = this.vmsList[i].zone
            break
          }
        }
        if (!this.volSpecifications) return false
        this.volSpecifications.forEach(item => {
          if (item.internalvolume_spec.attribute.Zone == computerZone) {
            this.selectVolumeList.push(item)
          }
        })
      }
    },
    changeVm (type, vm) {
      if (type == 'memory') {
        this.getVmSize(vm)
      }
    },

    // set虚拟机容量 得到虚拟机当前和最大
    getVmSize (vm_uuid) {
      const self = this
      const params = {
        cluster_uuid: this.cluster_uuid,
        compute_uuid: vm_uuid,
        tenant: self.cur_tenant
      }
      self.$Api.computers.vmInfo(params).then(response => {
        if (response.scode == 0) {
          const data = response.data
          self.vm.Max_Capacity = data.mem_max
          self.vm.Capacity = data.mem_size
        }
      })
    },

    // 挂载磁盘
    beforeSelectVolume (flag) {
      if (this.addForm.volume_type == 'out') {
        this.selectVolumeList = this.shareVolumeList
      } else {
        if (flag) {
          this.getVolumeList()
        }
      }
    },
    getVolumeList () {
      this.selectVolumeList = []
      if (!this.addForm.compute) {
        this.$message({
          message: this.$t('ticket.vmCannotEmpty'),
          type: 'error',
          duration: 1500
        })
        return false
      } else {
        let computerZone = ''
        for (let i = 0; i < this.vmsList.length; i++) {
          if (this.vmsList[i].uuid == this.addForm.compute) {
            computerZone = this.vmsList[i].zone
            break
          }
        }
        this.volumeList.forEach(item => {
          if (item.Attr.Zone == computerZone) {
            this.selectVolumeList.push(item)
          }
        })
      }
    },
    // 添加网卡
    beforeSelectSwitch (flag) {
      if (flag) {
        this.getNetSwitchList()
      }
    },
    getNetSwitchList () {
      this.selectSwitchList = []
      if (!this.addForm.compute) {
        this.$message({
          message: this.$t('ticket.vmCannotEmpty'),
          type: 'error',
          duration: 1500
        })
        return false
      } else {
        for (let i = 0; i < this.vmsList.length; i++) {
          if (this.vmsList[i].uuid == this.addForm.compute) {
            this.switches.forEach(item => {
              if (item.zone_uuid == this.vmsList[i].zone) {
                this.selectSwitchList.push(item)
              }
            })
            break
          }
        }
      }
    }
  },
  mounted () {
    if (this.showAddTicketDialog) {
      // 获取审批流程
      let processParmas = {
        tenant_uuid: this.cur_tenant,
        cluster_uuid: this.cluster_uuid
      }
      this.$Api.computers.ticketPrecesses(processParmas).then(res => {
        this.processes = res.data.list
      })
    }
  },
  watch: {
    showAddTicketDialog (val) {
    }
  }

}
</script>
<style lang="scss" scoped>
.ticket-button{
  // border-top: 1px solid #CBCBCB;
  padding-top: 10px;
  float: right;
}
.cpu-num{
  margin-bottom: 0px !important;position:relative;
  &::before{
    content: '*';
    color: #C62828; display: inline;
    position: absolute;
    left: 58px;
    top: 5px;
  }
}
.apply-error-info{
    height: 22px;font-size: 12px;
    color: #C62828;
    line-height: 22px;
}
.memory-label{
  /deep/.el-input-group__append{
    width: 80px !important;
    text-align: center !important;
}

}
</style>

export default{
  methods: {
    // 虚拟机设置cpu ---1.不能超过最大限制 2.不能超过租户当前的配额
    check_setVm_cpu (value) {
      if (!value) {
        this.vm_cpu_error = this.$t('ticket.cpuCannotEmpty')
      } else {
        const re = /^([1-9]\d*)$/
        if (re.test(value)) {
          const max_cpu = this.addForm.cpu_sockets * this.addForm.cpu_socket_count
          // let max_quote_cpu = this.allQuote.allCpu;
          // if(max_quote_cpu >= value){
          if (max_cpu !== 0) {
            this.addForm.Max_cpu = max_cpu
            if (max_cpu >= value) {
              this.vm_cpu_error = ''
            } else {
              this.vm_cpu_error = this.$t('ticket.cpuCannotOver') + max_cpu
            }
          } else {
            if (value <= 96) {
              this.vm_cpu_error = ''
            } else {
              this.vm_cpu_error = this.$t('ticket.cpuCannotOver96')
            }
          }
          // }else{
          //     this.vm_cpu_error = 'CPU核数不能超过租户CPU配额'+max_quote_cpu;
          // }
        } else {
          this.vm_cpu_error = this.$t('ticket.cpuhavetoBeNum')
        }
      }
    },
    // 检验输入cup插槽数 不大于4
    check_setVm_cpu_sockets (s) {
      const self = this
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s + this.$t('ticket.cannotEmpty')}`))
        } else {
          const re = /^([1-9]\d*)$/
          if (re.test(value)) {
            if (value <= 4) {
              self.check_setVm_cpu(self.addForm.cpu_count)
              return callback()
            } else {
              return callback(new Error(`${s + this.$t('ticket.cannotOver4')}`))
            }
          } else {
            return callback(new Error(`${s + this.$t('ticket.havetoNum')}`))
          }
        }
      }
    },
    // 检验输入的cpu插槽核数 不大于24
    check_setVm_cpu_socket_count (s) {
      const self = this
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s + this.$t('ticket.cannotEmpty')}`))
        } else {
          const re = /^([1-9]\d*)$/
          if (re.test(value)) {
            if (value <= 24) {
              self.check_setVm_cpu(self.addForm.cpu_count)
              return callback()
            } else {
              return callback(new Error(`${s + this.$t('ticket.cannotOver24')}`))
            }
          } else {
            return callback(new Error(`${s + this.$t('ticket.havetoNum')}`))
          }
        }
      }
    },
    // 虚拟机扩容-------------同时不能超过租户配额容量
    checkVmMemSize (s) {
      const self = this
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s + this.$t('ticket.cannotEmpty')}`))
        } else {
          const re = /^[1-9]\d*(\.\d{1}){0,1}$/
          const max_size = self.vm.Max_Capacity
          const newinput = self.addForm.vm_memory_unit === 'G' ? value * (1024 * 1024 * 1024) : value * (1024 * 1024 * 1024 * 1024)
          const max_name = max_size / (1024 * 1024 * 1024) + 'G'
          if (re.test(value) || String(value) === '0.5') {
            if (value % 0.5 == 0) {
              if (256 * (1024 * 1024 * 1024) >= newinput) {
                return callback()
              } else {
                return callback(new Error(`${s + this.$t('ticket.cannotOver256')}`))
              }
            } else {
              return callback(new Error(`${s + this.$t('ticket.havetoNum2')}`))
            }
          } else {
            return callback(new Error(`${s + this.$t('ticket.printAgain')}`))
          }
        }
      }
    },

    addTicket (type) {
      if (this.vm_cpu_error) {
        return false
      }
      this.$refs.addForm.validate((valid) => {
        if (valid) {
          switch (type) {
            case 'cpu':
              const submit_add_vm_cpu = this.get_params_Vm_CPU()
              this.check_setVm_cpu(this.addForm.cpu_count)
              if (!this.vm_cpu_error) {
                this.submit_info(submit_add_vm_cpu)
              }
              break
            case 'memory':
              const submit_add_vm_mem = this.get_params_Vm_Mem()
              this.submit_info(submit_add_vm_mem)
              break
            case 'mountDisk':
              const submit_add_vm_exist_disk = this.get_params_Vm_exist_disk()
              this.submit_info(submit_add_vm_exist_disk)
              break
            case 'addDisk':
              const submit_add_vm_disk = this.get_params_Vm_Disk()
              this.submit_info(submit_add_vm_disk)
              break
            case 'addNet':
              const submit_add_vm_net = this.get_params_Vm_Net()
              this.submit_info(submit_add_vm_net)
              break
          }
        }
      })
    },
    // 虚拟机设置CPU
    get_params_Vm_CPU () {
      const params_Vm_setCPU = {
        ticket_name: this.addForm.ticketName,
        reason: this.addForm.reason,
        process_uuid: this.addForm.process,
        type: 'VM_UPDATE_CPU_COUNT',						// 标志
        compute: {
          vm_cpu_update: {
            vm_uuid: this.addForm.compute, // 指定虚拟机
            cpu_count: parseInt(this.addForm.cpu_count),				// 想要的cpu个数
            cpu_max_count: parseInt(this.addForm.cpu_sockets) * parseInt(this.addForm.cpu_socket_count),
            cpu_sockets: parseInt(this.addForm.cpu_sockets),			// ？
            cpu_socket_count: parseInt(this.addForm.cpu_socket_count)
          }
        }
      }
      if (this.addForm.uuid) {
        params_Vm_setCPU.ticket_uuid = this.addForm.uuid
      }
      return params_Vm_setCPU
    },
    // 虚拟机内存修改
    get_params_Vm_Mem () {
      const params_Vm_setMem = {
        ticket_name: this.addForm.ticketName,
        reason: this.addForm.reason,
        process_uuid: this.addForm.process,
        type: 'VM_UPDATE_MEM_SIEZE',						// 标志
        compute: {
          vm_memory_update: {
            vm_uuid: this.addForm.compute, // 指定虚拟机
            vm_memory_size: this.addForm.vm_memory_unit === 'G' ? String(this.addForm.vm_memory_size * (1024 * 1024 * 1024)) : String(this.addForm.vm_memory_size * (1024 * 1024 * 1024 * 1024))
          }
        }
      }
      if (this.addForm.uuid) {
        params_Vm_setMem.ticket_uuid = this.addForm.uuid
      }
      return params_Vm_setMem
    },
    // 虚拟机挂载已存在的磁盘
    get_params_Vm_exist_disk () {
      return this.addForm.volume_type === 'inner' ? this.getTypeIsOn() : this.getTypeisOut()
    },
    getTypeIsOn () {
      const self = this
      const params = {
        ticket_name: self.addForm.ticketName,
        reason: self.addForm.reason,
        process_uuid: self.addForm.process,
        type: 'VM_DISK_EXIST_MOUNT',					// 标志
        compute: {
          vm_disk_exist_mount: {
            vm_uuid: self.addForm.compute, // 指定虚拟机
            target_bus: self.addForm.disk_type,
            inner_volume: {					// 内置卷
              volume_uuid: self.addForm.volume				// 卷 uuid
            }
          }
        }
      }
      if (self.addForm.uuid) {
        params.ticket_uuid = self.addForm.uuid
      }
      return params
    },
    getTypeisOut () {
      const self = this
      const volume = self.selectVolumeList.filter(item => {
        return item.uuid == self.addForm.volume
      })
      const params = {
        ticket_name: self.addForm.ticketName,
        reason: self.addForm.reason,
        process_uuid: self.addForm.process,
        type: 'VM_DISK_EXIST_MOUNT',					// 标志
        compute: {
          vm_disk_exist_mount: {
            vm_uuid: self.addForm.compute, // 指定虚拟机
            target_bus: self.addForm.disk_type,
            external_volume: {
              pool_uuid: volume[0].pool_uuid,
              volume_uuid: self.addForm.volume
            }
          }
        }
      }
      if (self.addForm.uuid) {
        params.ticket_uuid = self.addForm.uuid
      }
      return params
    },
    // 虚拟机挂载磁盘
    get_params_Vm_Disk () {
      const params_Vm_disk = {
        ticket_name: this.addForm.ticketName,
        reason: this.addForm.reason,
        process_uuid: this.addForm.process,
        type: 'VM_DISK_NOT_EXIST_MOUNT',					// 标志
        compute: {
          vm_disk_not_exist_mount: {
            pool_uuid: this.addForm.specification_pool,
            vm_uuid: this.addForm.compute, // 指定虚拟机
            volume_specification_uuid: this.addForm.specification_volume,
            target_bus: this.addForm.disk_type
          }
        }
      }
      if (this.addForm.uuid) {
        params_Vm_disk.ticket_uuid = this.addForm.uuid
      }
      return params_Vm_disk
    },
    // 虚拟机添加网卡
    get_params_Vm_Net () {
      let zoneUuid
      let switchName
      this.vmsList.forEach(item => {
        if (item.uuid == this.addForm.compute) {
          zoneUuid = item.zone
        }
      })
      this.selectSwitchList.forEach(item => {
        if (item.uuid == this.addForm.switch) {
          switchName = item.name
        }
      })
      const params_Vm_net = {
        ticket_name: this.addForm.ticketName,
        reason: this.addForm.reason,
        process_uuid: this.addForm.process,
        type: 'VM_NETWORK_CARD_ADD',						// 标志
        compute: {
          vm_network_card_add: {
            zone_uuid: zoneUuid,
            vm_uuid: this.addForm.compute, // 指定虚拟机
            network_card_type: this.addForm.netcard_type, // 网卡类型：可选参数virtio,e1000,rtl8139
            interface_type: this.addForm.internet_type,
            switch_uuid: this.addForm.internet_type === 'internal' ? this.addForm.switch : '',
            switch_name: switchName
          }
        }
      }
      if (this.addForm.uuid) {
        params_Vm_net.ticket_uuid = this.addForm.uuid
      }
      return params_Vm_net
    },
    submit_info (val) {
      val.cluster_uuid = this.cluster_uuid
      this.$Api.computers.submit_ticket(val).then(res => {
        if (res.scode == 0) {
          this.$message({
            message: this.$t('ticket.addOk'),
            type: 'success',
            duration: 1500
          })
          this.closeDialog()
        } else {
          this.$message({
            message: res.message_cn,
            type: 'warning',
            duration: 1500
          })
        }
      })
    },
    closeDialog () {
      if (this.netWebsocket) {
        this.netWebsocket.close()
      }
      this.$emit('closeAddTicketDialog', false)
    }
  }
}

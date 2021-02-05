import { mapState, mapActions, mapMutations } from 'vuex'
import axios from 'axios'
export default {
  data () {
    return {
      nowShutdown: false,
      afterShutdown: true,
      shutdownType: '2',
      getAgentState: null,
      isShowAdminAuthDialog: false

    }
  },
  computed: {
    ...mapState('Computers', ['cur_StartedVmList']),
    ...mapState('SystemConfig', ['isAdminRoler'])
  },
  // mixins: [Volumes, ServiceModule],
  methods: {
    ...mapMutations('Computers', ['changeStartedVmList']),
    // websocket通信处理
    socketMessage () {
      this.socket.onmessage = (msg) => {
        const res = JSON.parse(msg.data)
        // # 服务端消息类型
        // 0： agent连接成功, 不返回消息内容
        // 1： 拒绝agent连接  不返回消息内容，但topviewer要弹出scode报错信息。
        // 2： 消息通知   消息内容格式：{"message":""}
        // 3： agent关机   不返回消息内容
        // 4： agent重启    不返回消息内容
        // 5:  退出登录
        // 6:  虚拟机实时修改设置
        // 7:  个性化设置实施修改
        // 8:  升级包
        // 9： 重复登录
        // 10: 桌面池取消分配 需要刷新菜单列表
        // # topview消息类型
        // 0： agent注册 消息内容格式：{"id":"xx","description":"xxx","os_version":"xxx","api_key":"xx"}
        // 1： agent监控信息 待定
        // 11: 获取该agentId绑定的桌面池
        // 12: 桌面池下的绑定虚拟机有修改 set_type（unbind表示解绑，bind表示添加了绑定）
        // 16: 虚拟机有操作 通知对应的页面刷新
        // 17: 卷有操作
        if (res.scode == 0) {
          const type = res.type
          const data = res.data
          // type !== 0 && type !== 6 && type !== 7 && type !== 8 && type !== 10 && type !== 11 && type !== 12 && type !== 16 && type !== 17

          if (type && [1, 2, 3, 4, 5, 9].includes(type)) {
            var tipInfo = type == 3 ? this.$t('wb.shutdownTip') : type == 4 ? this.$t('wb.rebootTip') : type == 5 ? this.$t('wb.reconnectionTip') : type == 9 ? this.$t('wb.repeatLogin') : type == 2 ? data.message : this.language == 'zh' ? res.message_cn : res.message
            if (type == 2) {
              this.setState({
                attr: 'noticeMsg',
                val: {
                  type: type,
                  msg: tipInfo
                }
              })
              this.setState({
                attr: 'isOpenNoticeDialog',
                val: true
              })
            }
          }
          if (type == 3 || type == 4) { // 接收到关机/重启信息，需要用户自己选择是否执行
            this.setState({
              attr: 'optNoticeDialog',
              val: {
                isShow: true,
                optType: type == 3 ? '关机' : '重启',
                info: tipInfo,
                time: this.optNoticeDialog.time,
                exeFlag: true
              }
            })
            setTimeout(() => {
              // 判断是否执行
              if (this.optNoticeDialog.exeFlag) {
                let Api = this.optNoticeDialog.optType == '关机' ? this.$Api.system.agentPoweroff : this.$Api.system.agentReboot
                this.setState({
                  attr: 'optNoticeDialog',
                  val: {
                    isShow: false,
                    optType: '',
                    info: '',
                    time: 300,
                    exeFlag: true
                  }
                })
                Api({Delay: 0}).then(res => {
                  console.log('res', res)
                })
                this.closeSocket()
              }
            }, 300000)
          }
          if (type == 5 || type == 9 || type == 1) {
            // 需要返回登录的
            // 如果是集群云桌面客户端超过授权最大数量，需要缓一点时间让他能切换结群
            let switchFlag = Boolean(type == 1 && res.message_cn.includes('超过授权'))
            setTimeout(() => {
              this.backLogin(tipInfo)
            }, switchFlag ? 3000 : 0)
          }
          if (type == 6) {
            // 虚拟机实时修改设置
            let setUrl
            let vmSettingParams
            if (data.resolution) {
              setUrl = '/top-viewer/resize'
              vmSettingParams = {resolution: data.resolution.replace('x', '*')}
            }
            if (data.image_compression) {
              setUrl = '/top-viewer/change_image_compression'
            }
            // if (data.ban_input == true || data.ban_input == false) {
            //   setUrl = '/top-viewer/disinput'
            //   vmSettingParams = {disinput: data.ban_input ? 'true' : 'false'}
            // }  // 禁止输入的设置暂时取消
            if (data.video_compression) {
              setUrl = '/top-viewer/change_video_code'
              vmSettingParams = {videocode: data.video_compression}
            }
            if (this.cur_guide_item.exclusive_desktop_uuid == data.exclusive_desktop_uuid) {
              // 如果当前所处桌面池，与修改的桌面池一样，刷新页码
              this.searchIndex('刷新页码')
              this.$message({
                message: this.$t('system.userDesktopSettingEdit'),
                type: 'warning',
                duration: 1500
              })
            }
            // if(this.cur)
            if (!setUrl) return false
            this.cur_StartedVmList.forEach(item => {
              if (data.exclusive_desktop_uuid == item.exclusive_desktop_uuid) {
                let url = ':' + item.HttpPort + setUrl
                axios.post(url, vmSettingParams).then(res => {
                })
              }
            })
          }
          if (type == 7) { // 个性化设置实时修改
            this.changeSetting(data)
          }
          if (type == 8) { // 升级
            this.upgrade(data)
          }
          // 桌面池编辑有操作(分配用户)
          if (type == 10) {
            if (this.$route.name == 'desktopVms') {
              for (let user in data) {
                if (user == this.useruuid) {
                  // 用户桌面池分配已被修改
                  this.searchIndex('刷新页码')
                  this.$message({
                    message: this.$t('system.userDesktopAssignEdit'),
                    type: 'warning',
                    duration: 1500
                  })
                }
              }
            }
          }
          if (type == 11) { // 存可操作的数据
            this.setState({
              attr: 'bindVms',
              val: Object.entries(res.data.bind_vm)
            })
          }
          if (type == 12) { // 桌面池下的虚拟机绑定情况有修改
            if (this.cur_guide_item.exclusive_desktop_uuid == res.data.desktop_pool_uuid) { // 修改了这个池下的虚拟机
              this.searchIndex('刷新页码')
              this.$message({
                message: this.$t('system.userDesktopbindEdit'),
                type: 'warning',
                duration: 1500
              })
            }
          }
          if (type == 16) {
            // 虚拟机有操作
            this.getVmOptChange(data)
          }
          if (type == 17) {
            // 虚拟机有操作
            this.getVolumeOptChange(data)
          }
        } else {
          console.error(res.message)
        }
      }
    },
    backLogin (tipInfo) { // 返回登录页面
      this.setState({
        attr: 'isKickedOut',
        val: {
          msg: tipInfo,
          value: true
        }
      })
      this.closeSocket()
      this.$router.push('/Login')
      this.$confirm(tipInfo, this.$t('resource.tip'), {
        confirmButtonText: this.$t('resource.confirm'),
        closeOnClickModal: false,
        showCancelButton: false,
        type: 'warning',
        callback: () => {
          this.setState({
            attr: 'isKickedOut',
            val: {
              msg: '',
              value: false
            }
          })
          sessionStorage.clear()
        }
      })
    },
    changeSetting (data) { // 个性化设置实时修改
      let changeUserFlag = ''
      let changePoolFlag = ''
      // 只要设置策略有修改 就会无差别推送给客户机
      // 如果用户已有设置，收到推送时先比较收到的策略uid是否相同，如果不同，就不进行设置修改
      if (this.setting.uuid && this.setting.uuid !== data.uuid) {
        return false
      }
      data.associated_user.forEach(item => {
        if (item.user_uuid == this.useruuid) { // 如果修改的内容里有当前用户
          changeUserFlag = 1
          this.setState({
            attr: 'setting',
            val: data
          })
        }
      })
      let settingInfo = ''
      if (changeUserFlag == 1) { // 用户设置修改了
        // 修改路由
        if ((this.$route.path == '/desktopVms' && !this.setting.show_cloud_desktop) || (this.$route.path == '/netdisk' && !this.setting.show_cloud_storage)) {
          this.$router.push('/computer')
          this.setState({
            attr: 'cur_guide_item',
            val: 'vm_page'
          })
        }
        // 如果此时有拉起的虚拟机
        if (this.cur_StartedVmList.length !== 0) {
          if (this.setting.enable_screen_watermark) {
            settingInfo = this.setting.watermark_content + ',' + this.setting.watermark_move_time + ',' + (this.setting.lean ? this.setting.lean : 'normal') + ',' + this.setting.font_size + ',' + this.setting.font_thickness + ',' + this.setting.content_color + ',' + this.setting.show_style + ',' + (this.setting.underline_type ? this.setting.underline_type : 'none') + ',' + (this.setting.background_color ? this.setting.background_color !== 'transparent' ? (this.setting.background_color + ',0') : 'pink,1.0' : 'none') + ',' + this.setting.watermark_coordinate_x + ',' + this.setting.watermark_coordinate_y

            // 遍历当前启用的虚拟机,去设置
            this.cur_StartedVmList.forEach(item => {
              this.setVmSetting(item.HttpPort, settingInfo)
            })
          }
        }
      } else {
        // 无用户设置时,重置用户设置
        this.setState({
          attr: 'setting',
          val: { show_cloud_desktop: true, show_cloud_server: true, show_cloud_storage: true }
        })
        this.setState({ // 给桌面池设置赋值
          attr: 'poolSetting',
          val: data
        })
        // 在无用户设置,且桌面池被修改的情况下
        settingInfo = data.watermark_content + ',' + data.watermark_move_time + ',' + (data.lean ? data.lean : 'normal') + ',' + data.font_size + ',' + data.font_thickness + ',' + data.content_color + ',' + data.show_style + ',' + (data.underline_type ? data.underline_type : 'none') + ',' + (data.background_color ? data.background_color !== 'transparent' ? (data.background_color + ',0') : 'pink,1.0' : 'none,0')
        let cur_startedVmList = JSON.parse(JSON.stringify(this.cur_StartedVmList)) // 存放vuex的值 以便修改
        cur_startedVmList.forEach(vm => {
          let clearFlag = ''
          data.associated_desktop_pool.forEach(pool => {
            if (vm.exclusive_desktop_uuid == pool.desktop_pool_uuid && !vm.setting_uuid) {
              vm.setting_uuid = data.uuid
              this.changeStartedVmList({data: cur_startedVmList, type: 'eval'})
            }
            if (vm.setting_uuid == data.uuid && pool.desktop_pool_uuid == vm.exclusive_desktop_uuid) {
              // this.setVmSetting(vm.HttpPort, settingInfo)
              clearFlag = 1
            }
          })
          if (!clearFlag) { // 如果设置的池不包含当前使用的池,置空
            this.setVmSetting(vm.HttpPort, ",0,oblique,28,20,DodgerBlue,normal,none,''")
          } else {
            this.setVmSetting(vm.HttpPort, settingInfo)
          }
        })
      }
      // 当前终端设置已修改
      this.$message({
        message: this.$t('system.terminalEditTip'),
        type: 'warning',
        duration: 1500
      })
    },
    // 收到升级的命令
    upgrade (data) {
      this.isShowAdminAuthDialog = true
      // this.setState({
      //   attr: 'isKickedOut',
      //   val: {
      //     msg: this.$t('resource.readyUpgrade'),
      //     value: true
      //   }
      // })
      // this.$confirm(this.$t('resource.readyUpgrade'), this.$t('resource.tip'), {
      //   confirmButtonText: this.$t('resource.confirm'),
      //   closeOnClickModal: false,
      //   closeOnPressEscape: false,
      //   showClose: false,
      //   showConfirmButton: false,
      //   showCancelButton: false,
      //   type: 'warning',
      //   callback: () => {
      //   }
      // })
      // 先要调用以管理员运行的接口,这个接口会拒绝连接，但别给提示 （使用upgradeFlag）
      // 然后测试agent是否重新拉起
      // 确定拉起后 再调用升级的接口
      localStorage.setItem('noErrorTipFlag', true)
      this.$Api.system.hostMsg().then(res => {
        localStorage.removeItem('noErrorTipFlag')
        if (res.Uac) {
          this.confirmUpgrade(data)
        } else {
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
                this.isShowAdminAuthDialog = false
                if (res.Uac) {
                  this.confirmUpgrade(data)
                } else {
                  this.$message({
                    message: this.$t('setting.noAuth'),
                    type: 'warning',
                    duration: 3000
                  })
                }
              })
            }, 5000)
          })
        }
      }).catch(() => {
        localStorage.removeItem('noErrorTipFlag')
        this.$message({
          message: this.$t('login.agentTip'),
          type: 'warning',
          duration: 3000
        })
      })
    },
    confirmUpgrade (data) {
      const parmas = {
        Hosts: data.host_ip,
        PackagePath: data.update_file_path,
        PackageName: data.update_file_name
      }
      this.$Api.system.upgradePack(parmas).then(res => {
        this.closeSocket()
      })
    },
    setVmSetting (HttpPort, settingInfo) {
      let url = ':' + HttpPort + '/top-viewer/change_watermark'
      axios.post(url, {watermark: settingInfo}).then(res => {
      })
    },
    getVmOptChange (data) {
      if (data.exclusive_desktop_uuid) {
        // 对桌面池进行的操作
        if (this.cur_guide_item.exclusive_desktop_uuid == data.exclusive_desktop_uuid) { // 当前所在池是被操作池，需要刷新页面
          this.$message({
            message: '当前桌面池内容已被修改!',
            type: 'warning',
            duration: 1500
          })
          this.setState({
            attr: 'reFresh_cur_page',
            val: Math.random()
          })
        }
      }
    },
    getVolumeOptChange (data) {
      if (data.volume_uuid || data.volume_uuid.length > 0) {
        // 对卷操作了
        var changeFlag = false
        data.volume_uuid.forEach(item => {
          if (item == this.cur_guide_item.volume_uuid) {
            changeFlag = true
          }
        })
        if (!this.cur_guide_item) { // 卷为空时
          changeFlag = true
        }
        if (changeFlag) {
          this.$message({
            message: '当前存储卷已被修改!',
            type: 'warning',
            duration: 1500
          })
          this.setState({
            attr: 'reFresh_cur_page',
            val: Math.random()
          })
        }
      }
    }
  },
  beforeDestroy () {
    clearInterval(this.getAgentState)
    this.getAgentState = null
  }
}

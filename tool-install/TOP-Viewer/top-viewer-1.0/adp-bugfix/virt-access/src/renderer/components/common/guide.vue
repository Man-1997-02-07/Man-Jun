<template>
  <div class="desktop-guide-list">
    <!-- v-show="setting.show_cloud_desktop" -->
    <div
      v-show="setting.show_cloud_desktop"
      class="desktop-guide"
      :class="{'desktop-guide-cover':cur_guide=='desktop'}"
      @click="getGuide('desktop')">
      <i class="sg-iconfont im-icon-yunzhuomian" />{{ $t('resource.cloudDesk') }}
      <i
        :style="$i18n.locale=='zh'?'padding-left:102px;':'padding-left:57px;'"
        :class="[cur_guide_cont=='desktop'?'el-icon-arrow-down':'el-icon-arrow-right']" />
    </div>
    <div v-show="cur_guide_cont=='desktop'">
      <div
        v-if="guideDesktopList.length==0"
        class="guide-nodata">
        {{ $t('resource.noData') }}
      </div>
      <vx-scroll
        v-if="guideDesktopList.length>0"
        :init="true"
        style="height: 500px;">
        <div>
          <div
            v-for="(item,index) in guideDesktopList"
            :key="index"
            :class="['guide-vm-cont',{'active':item.exclusive_desktop_uuid==cur_guide_item.exclusive_desktop_uuid}]"
            @click.stop="getDesktopVms(item)">
            <img
              src="@/assets/images/icons/vm-icon.svg">
            <el-tooltip
              effect="light"
              :content="item.exclusive_desktop_name">
              <span v-if="item.exclusive_desktop_name.length<10">{{ item.exclusive_desktop_name }}</span>
              <span v-else>{{ item.exclusive_desktop_name.substr(0,10) }}...</span>
            </el-tooltip>
            <i
              v-if="item.vm_template_os_type=='windows'"
              class="sg-iconfont im-icon-windows" />
            <i
              v-else
              class="sg-iconfont im-icon-linux" />
          </div>
        </div>
      </vx-scroll>
    </div>
    <!--云服务器
    v-show="setting.show_cloud_server"-->
    <!-- &&setting.show_cloud_server
     -->
    <div
      v-if="isShow_VmGuide_flag"
      class="desktop-guide"
      :class="{'desktop-guide-cover':cur_guide=='vm'}"
      @click="getGuide('vm')">
      <i class="sg-iconfont im-icon-yunzhuji" />{{ $t('resource.cloudServers') }}
      <i
        :style="$i18n.locale=='zh'?'padding-left:85px;':'padding-left:62px;'"
        :class="[cur_guide_cont=='vm'?'el-icon-arrow-down':'el-icon-arrow-right']" />
    </div>
    <div v-show="cur_guide_cont=='vm'">
      <div
        v-if="guideVmList.length==0"
        class="guide-nodata">
        {{ $t('resource.noData') }}
      </div>
      <vx-scroll
        v-if="guideVmList.length>0"
        :init="true"
        style="height: 500px;">
        <div>
          <div
            v-for="(item,index) in guideVmList"
            :key="index"
            :class="['guide-vm-cont',{'active':item.uuid==cur_guide_item.uuid}]"
            @click.stop="getvmScreenShot(item)">
            <img
              v-if="item.state=='shutoff'"
              src="@/assets/images/vm/vmIcon-shutoff.png">
            <img
              v-else
              src="@/assets/images/vm/vmIcon-open.png">
            <el-tooltip
              effect="light"
              :content="item.name">
              <span v-if="item.name.length<10">{{ item.name }}</span>
              <span v-else>{{ item.name.substr(0,10) }}...</span>
            </el-tooltip>
            <i
              v-if="item.os.includes('Windows')"
              class="sg-iconfont im-icon-windows" />
            <i
              v-if="item.os.includes('Ubuntu')||item.os.includes('CentOS')||item.os.includes('Fedora')||item.os.includes('Linux')"
              class="sg-iconfont im-icon-linux" />
          </div>
        </div>
      </vx-scroll>
    </div>
    <!-- 云存储导航 -->
    <div
      v-show="setting.show_cloud_storage"
      :class="['desktop-guide',{'desktop-guide-cover':cur_guide=='storage'}]"
      @click="getGuide('storage')">
      <i
        class="sg-iconfont im-icon-yuncunchu"
        style="margin-right:5px;" />{{ $t('resource.cloudStorage') }}<i
          :style="$i18n.locale=='zh'?'padding-left:102px;':'padding-left:63px;'"
          :class="[cur_guide_cont=='storage'?'el-icon-arrow-down':'el-icon-arrow-right']" />
    </div>
    <div v-show="cur_guide_cont=='storage'">
      <div
        v-if="guideS3VolumeList.length==0"
        class="guide-nodata">
        {{ $t('resource.noData') }}
      </div>
      <div
        v-for="(item,index) in guideS3VolumeList"
        v-else
        :key="index">
        <div
          :class="['guide-storage-cont',{'active':item.volume_uuid==cur_guide_item.volume_uuid}]"
          @click.stop="getS3Files(item)">
          <i
            class="sg-iconfont im-icon-yunpan"
            style="margin-right:10px;" />
          <el-tooltip
            effect="light"
            :content="item.volume_name">
            <span v-if="item.volume_name.length<10">{{ item.volume_name }}</span>
            <span v-else>{{ item.volume_name.substr(0,10) }}...</span>
          </el-tooltip>
        </div>
      </div>
    </div>
  </div>
</template>
<script>
import { mapState, mapActions } from 'vuex'
export default {
  data () {
    return {
      isShowDesktop: true,
      cur_guide: 'desktop', // 当前导航栏
      cur_guide_cont: 'desktop', // 当前导航展开的内容
      guideDesktopList: '',
      isShow_VmGuide_flag: !sessionStorage.getItem('systemMode'), // 云桌面模式下不展示云服务器菜单
      guideVmList: [],
      guideS3VolumeList: [],
      cur_vm: ''
    }
  },
  methods: {
    getGuide (val) {
      if (this.cur_guide == val) {
        if (!this.cur_guide_cont) {
          this.cur_guide_cont = val
        } else {
          this.cur_guide_cont = '' // 将展开的内容栏项收起，但单行title要保持选中状态
        }
      } else {
        this.cur_guide = val
        this.cur_guide_cont = val
        this.getGuideInfo(val)
      }
    },
    getGuideInfo (val) { // 获取导航的内容
      if (val === 'desktop') {
        this.getDesktops()
        this.setState({
          attr: 'cur_guide_item',
          val: ''
        })
      }
      if (val === 'storage') {
        this.getS3VolumeGuideList()
      }
      if (val === 'vm') {
        this.getGuideVms()
      }
    },
    getDesktops (val) {
      // this.guideDesktopList = []
      const param = {
        tenant: this.cur_tenant,
        page_num: 0,
        page_size: 0,
        filter_name: this.search_val,
        cluster_uuid: this.cluster_uuid
      }
      this.$Api.desktop.desktopList(param).then(res => {
        if (res.scode == 0) {
          this.guideDesktopList = res.data.list
          if (this.guideDesktopList.length !== 0) {
            this.setState({
              attr: 'cur_guide_item',
              val: this.guideDesktopList[0]
            })
          }
          if (this.guideDesktopList.length == 0) { // 如果结果为空
            this.setState({
              attr: 'cur_guide_item',
              val: 'block_res'
            })
          }
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
        this.$router.push('/desktopVms')
      })
    },
    // 点击云桌面 获取云桌面下的虚拟机
    getDesktopVms (val) {
      this.setState({
        attr: 'cur_guide_item',
        val: val
      })
      this.$router.push('/desktopVms')
    },
    getGuideVms (val) {
      // this.guideVmList = []
      const param = {
        tenant: this.cur_tenant,
        page_num: 0,
        page_size: 0,
        filter_fuzzy: this.search_val !== '刷新页码' ? this.search_val : '',
        filter_host_type: 'GUS,GVPROXY',
        // filter_user_uuid: this.useruuid,
        cluster_uuid: this.cluster_uuid
      }
      this.setState({
        attr: 'onLoading',
        val: true
      })
      if (this.isuser) {
        param.filter_user_uuid = sessionStorage.getItem('useruuid')
      }
      this.$Api.computers.vmList(param).then(res => {
        this.setState({
          attr: 'onLoading',
          val: false
        })
        if (res.scode == 0) {
          this.guideVmList = res.data.domains
          this.setState({ // 便于修改设置后切换路由使用
            attr: 'cur_guide_item',
            val: 'vm_page'
          })
          this.$router.push('/computer')
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    // 云盘
    getS3VolumeGuideList (val, searchFlag) {
      this.$nprogress.start()
      this.$Api.netdisk.getS3VolumeList({
        user: this.useruuid,
        page_num: 0,
        page_size: 0,
        filter_name: this.search_val,
        tenant: this.cur_tenant,
        cluster_uuid: this.cluster_uuid
      }).then(res => {
        this.$nprogress.done()
        if (res.scode == 0) {
          this.guideS3VolumeList = res.data.list
        }
        if (this.search_val && this.guideS3VolumeList.length == 0) { // 如果搜索结果为空
          this.setState({
            attr: 'cur_guide_item',
            val: 'block_res'
          })
        }
        // 如果来自公共搜到栏的内容，对应路由的传参页需要修改
        if (searchFlag && this.guideS3VolumeList.length !== 0) {
          this.setState({
            attr: 'cur_guide_item',
            val: this.guideS3VolumeList[0]
          })
        }
        if (this.guideS3VolumeList.length !== 0) {
          this.setState({
            attr: 'cur_guide_item',
            val: this.guideS3VolumeList[0]
          })
        } else {
          this.setState({
            attr: 'cur_guide_item',
            val: ''
          })
        }
        this.$router.push('/netdisk')
      })
    },
    getS3Files (val) {
      this.setState({
        attr: 'cur_guide_item',
        val: val
      })
      this.$router.push('/netdisk')
    },
    // 跳转屏幕截图
    getvmScreenShot (item) {
      this.setState({
        attr: 'cur_guide_item',
        val: item
      })
      this.$router.push('/vmScreenShot')
    }
  },
  created () {
    this.cur_guide = this.setting.show_cloud_desktop ? 'desktop' : 'vm'
  },
  mounted () {
    this.getDesktops()
  },
  watch: {
    cluster_uuid (val) { // 集群切换后刷新内容
      this.getGuideInfo(this.cur_guide)
    },
    cur_guide_item (val) {
      if (val == 'vm_page') {
        this.cur_guide = 'vm'
      }
    },
    search_val (val) {
      val = val.replace(/\s+/g, '')
      // 公共搜索栏 搜索导航内容
      setTimeout(() => {
        let cur_path = this.$route.path
        if (cur_path == '/desktopVms') {
          if (this.search_val == '刷新页码') {
            this.searchIndex('')
          } else {
            this.getDesktops(val)
          }
        }
        if (cur_path == '/computer' || cur_path == '/vmScreenShot') {
          this.getGuideVms(val)
          this.$router.push('/computer')
        }
        if (cur_path == '/netdisk') {
          this.getS3VolumeGuideList(val, true)
        // this.$router.push('/netdisk')
        }
      }, 150)
    },
    reFresh_cur_page (val) {
      let cur_path = this.$route.path
      if (cur_path == '/netdisk') {
        this.cur_guide = 'storage'
        this.cur_guide_cont = 'storage'
        this.getS3VolumeGuideList()
        // this.$router.push('/netdisk')
      }
    }
  }

}
</script>
<style lang="scss" scoped>
.desktop-guide-list{
    height: 100%;
    background-color: #F9F9F9;
}
.desktop-guide{
    height: 40px;cursor: pointer;
    border-bottom: 1px solid #EEEEEE;
    line-height: 40px;
    i{
      padding-left: 10px;padding-right: 10px;
    }
}
.desktop-guide:hover{
  background: #3583E3;
  color:#EEEEEE;
}
.desktop-guide-cover{
  background: #3583E3;
  color:#EEEEEE;
}
.active{
  background-color: #E8F2FF;
  span{
    font-weight: 800;
  }
}
.guide-vm-cont{
  padding:20px 12px 20px 10px;
  height:40px;
  // position: relative;
  cursor: pointer;
  img{
    height: 40px;
  }
  span{
    font-size:14px;
    color:#323132;
    position: absolute;
    padding-left: 10px;
  }
  i{
    color:#D6D6D6;
    padding-left: 10px;

  }
}
.guide-vm-cont:hover{
  background-color: #E8F2FF;
}
.guide-nodata{
    height: 500px;
    text-align: center;
    line-height: 90px;
    border-bottom: 1px solid #EEEEEE;
}
.sg-iconfont{
  width: 30px;
}
.guide-storage-cont{
  height: 40px;
  line-height: 40px;
  padding-left: 35px;
  // background-color: #E8F2FF;

  cursor: pointer;
}
.im-icon-linux{
  color: #000000 !important;
}
</style>

<template>
  <div>
    <div class="desktop-title">
      <span class="desktop-span">
        {{ $t('version.title') }}
      </span>
      <div style="display: inherit;">
        <div
          v-if="clusterList.length>1"
          class="cluster-cont"
          @click="showCluster">
          <el-tooltip content="切换集群">
            <i
              class="im-icon-jiqun sg-iconfont" />
          </el-tooltip>
          {{ clustername }}
          <i
            :class="isShowClusterList?'el-icon-caret-bottom':'el-icon-caret-right'"
            style="margin-top: 18px; margin-left: 2px;" />
        </div>
        <el-dropdown
          class="desktop-user"
          trigger="click">
          <span class="el-dropdown-link">
            <img
              src="@/assets/images/icons/user.png"
              class="avatar">
            <el-tooltip
              v-if="username.length > 10"
              placement="top"
              effect="light">
              <div>{{ username }}</div>
              <i class="username">{{ username || '' }}</i>
            </el-tooltip>
            <i
              v-else
              class="username">{{ username || '' }}</i>
            <i class="el-icon-caret-bottom el-icon--right" />
          </span>
          <el-dropdown-menu slot="dropdown">
            <el-dropdown-item
              v-if="!enable_ldap"
              id="top_cur_msg"
              divided
              @click.native="setPassword">
              <i class="sg-iconfont im-icon-shezhi1 dropdown-icon" />{{ $t('password.modifyPw') }}
            </el-dropdown-item>
            <el-dropdown-item
              id="top_cur_msg"
              divided
              @click.native="getCurInfos">
              <i class="im-icon-ms dropdown-icon" />{{ $t('resource.curInfo') }}
            </el-dropdown-item>
            <el-dropdown-item
              id="top_cur_msg"
              divided
              @click.native="switchLang">
              <i class="sg-iconfont  im-icon-shuaxin dropdown-icon" />{{ $t('resource.switchLang') }}
            </el-dropdown-item>
            <el-dropdown-item
              id="loginOut"
              divided
              @click.native="loginout">
              <i class="im-icon-exit dropdown-icon" />{{ $t('resource.signOut') }}
            </el-dropdown-item>
          </el-dropdown-menu>
        </el-dropdown>
      </div>
      <!-- 切换集群选择 --->
      <div
        v-if="isShowClusterList"
        class="cluster-list-cont"
        @mouseleave="showCluster">
        <div
          v-for="(item,index) in clusterList"
          :key="index"
          :class="['list-cont',cur_cluster.uuid==item.uuid?'active':'']"
          @click="switchCluster(item)">
          <i class="sg-iconfont im-icon-jiqun icon" />
          <el-tooltip
            :content="item.name"
            placement="top"
            effect="light">
            <span>{{ item.name.substr(0,18) }}</span>
          </el-tooltip>
          <span v-if="item.name.length>18">...</span>
          <i
            v-show="cluster_uuid==item.uuid"
            class="el-icon-check cluster-choosed" />
        </div>
        <div class="cluster-confirm-btn">
          <el-button
            type="primary"
            @click="confirmChangeCluster">
            {{ $t('resource.confirm') }}
          </el-button>
        </div>
      </div>
    </div>

    <div class="desktop-seachlabel">
      <el-input
        id="desktop-search"
        v-model="filter_fuzzy"
        @keyup.enter.native="searchInfos">
        <!--
        placeholder="搜索桌面" @keyup.enter.native="getComputerList"  @click="getComputerList" -->
        <el-button
          slot="prepend"
          icon="el-icon-search"
          style="background-color:#F9F9F9;"
          @click="searchInfos" />
      </el-input>
    </div>
    <el-dialog
      id="topLabel_current_info_dialog"
      class="current-info-dialog"
      :title="$t('resource.curInfo')"
      :visible.sync="showCurInfosDialog"
      :show-close="false"
      :close-on-click-modal="false"
      width="400px">
      <el-row>
        <span>{{ $t('resource.curUser') }}：</span>{{ username }}
      </el-row>
      <el-row>
        <span>{{ $t('resource.curCluseter') }}：</span>{{ clustername }}
      </el-row>
      <!-- <el-row>
        <span>{{ $t('resource.curClusterIP') }}：</span>{{ clustername }}
      </el-row> -->
      <el-row>
        <span>{{ $t('resource.curHostIp') }}：</span>{{ cur_hostIp }}
      </el-row>
      <el-row>
        <span>{{ $t('resource.curAgentId') }}：</span>{{ agentNameId }}
      </el-row>
      <el-row>
        <span>{{ $t('resource.curAgentVersion') }}：</span>{{ agentVersion }}
      </el-row>
      <el-row>
        <span>{{ $t('resource.curClientVersion') }}：</span>2.0.6
      </el-row>
      <el-row style="float: right;">
        <el-button @click="showCurInfosDialog=false">
          {{ $t('resource.close') }}
        </el-button>
      </el-row>
      <div style="clear:both;" />
    </el-dialog>

    <!-- 修改密码 -->
    <modify-passwd-dialog :show.sync="isShowPasswdDialog" />
  </div>
</template>

<script>
import { mapState, mapActions } from 'vuex'
import Basic from '@/test/basic.js'
import modifyPasswdDialog from './modifyPasswdDialog.vue'
var fs = require('fs')
export default {
  mixins: [Basic],
  inject: ['reloadRouter'],
  components: {modifyPasswdDialog},
  data () {
    return {
      desktopValue: '',
      filter_fuzzy: '',
      computerList: [],
      cur_computer: {
        name: ''
      },
      cur_cluster: {
        uuid: this.cluster_uuid,
        name: this.clustername
      },
      curClientVersion: '未知',
      showCurInfosDialog: false,
      isShowPasswdDialog: false,
      isShowClusterList: false // 展示集群列表
    }
  },
  methods: {
    loginout () {
      this.$nprogress.start()
      this.$confirm(this.$t('resource.checkSignout'), this.$t('resource.tip'), {
        closeOnClickModal: false,
        closeOnPressEscape: false
      }).then(() => {
        setTimeout(() => {
          this.closeSocket()
          this.$router.push('/Login')
        }, 100)
        this.$Api.userOperation.userLogout({cluster_uuid: this.cluster_uuid}).then(response => {
          this.$nprogress.done()
        })
      })
    },
    // 切换集群
    switchCluster (item) {
      this.cur_cluster = item
    },
    confirmChangeCluster () {
      if (this.cur_cluster.uuid == this.cluster_uuid) { // 没有切换就点确定
        return false
      }
      this.setState([{
        attr: 'clustername',
        val: this.cur_cluster.name
      }, {
        attr: 'cluster_uuid',
        val: this.cur_cluster.uuid
      }])
      sessionStorage.setItem('clustername', this.cur_cluster.name)
      sessionStorage.setItem('cluster_uuid', this.cur_cluster.uuid)
      this.reloadRouter()
      this.$message({
        message: this.$t('setting.changeClusterSuccess'),
        type: 'success',
        duration: 1500
      })
    },
    showCluster () {
      this.isShowClusterList = !this.isShowClusterList
      this.cur_cluster = {}
      this.cur_cluster.uuid = this.cluster_uuid
    },
    getCurInfos () {
      this.showCurInfosDialog = true
    },
    // 切换语言
    switchLang () {
      let locale = this.language
      locale == 'zh' ? locale = 'en' : locale = 'zh'
      this.setState({
        attr: 'language',
        val: locale
      })
      localStorage.setItem('language', locale) // 后面会用做切换和将用户习惯存储到本地浏览器
      this.$i18n.locale = locale
    },
    searchInfos () {
      this.searchIndex(this.filter_fuzzy)
    },
    setPassword () { // 修改密码
      this.isShowPasswdDialog = true
    },
    getClientVersion () {
      var content = JSON.parse(fs.readFileSync('./package.json', 'utf8'))
      var os = require('os')
      if (os.platform().includes('win')) {
        this.curClientVersion = content.version
      } else {
        this.curClientVersion = content.linuxVersion
      }
    }

  },
  mounted () {
    // this.getClientVersion()
  },
  watch: {
    search_val (val) {
      this.filter_fuzzy = val
    },
    clusterList (val) {
    }
    // $route (to, from) {
    //   // 路由变化 清掉公共搜索的值
    //   this.filter_fuzzy = ''
    //   this.setState({
    //     attr: 'search_val',
    //     val: ''
    //   })
    // }
  },
  computed: {
    ...mapState('SystemConfig', ['hostId', 'agentVersion', 'agentNameId']),
    clusterList () {
      return JSON.parse(sessionStorage.getItem('clusterList')) || []
    }
  }
}
</script>
<style  lang="scss" scoped>
.desktop-title{
  height:50px;
  color:#fff;display: flex;
  justify-content: space-between;
  background-color:#404750;
  line-height: 50px;    padding-right: 10px;
  .desktop-span{
    padding-left: 18px;
    font-size: 24px;
  }
  .avatar {
    height: 35px;
    vertical-align: middle;
    margin-right: 12px;
  }
  .desktop-user{
    color:#fff;

  }
}
.desktop-seachlabel{
  padding: 10px;
  border-bottom: 1px solid #E7E7E7;
}
.username{
  cursor: pointer;
}
.current-info-dialog{
  .el-row{
    height: 40px;line-height: 40px;
    span{
      width: 120px;
      display: inline-block;
      text-align: right;
      margin-right: 10px;
    }
  }
}
.cluster-cont{
  display: inline-flex;    margin-right: 30px;cursor: pointer;
  .im-icon-jiqun{font-size: 28px;
    font-weight: 600;
    margin-right: 5px;
    margin-top: 2px;
  }
}
.cluster-list-cont{
    position: absolute;
    top: 50px;
    color: #181D28;
    background: #fff;
    z-index: 100;
    right: 100px;
    width: 187px;
    border: 1px solid #e0e0e0;
    border-radius: 2px;
    .list-cont{
        margin: 5px;padding-left: 5px;
        cursor: pointer;
        height: 30px;    line-height: 30px;
        span{
          vertical-align: super;
        }
        &:hover{
          color:#77aae9 ;
        }
    }
}
.cluster-choosed{
  font-size: 18px;
  float: right;
  padding-right: 11px;
  padding-top: 4px;
}
.icon {
    font-size: 24px;
}
.active{
  background-color: #3583E3;color:white;
  border-radius: 15px;
}
.cluster-confirm-btn{
  margin-left: 120px;
}
</style>

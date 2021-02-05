<template>
  <div style="height: 100%;background-color: #21213f;">
    <div v-if="osType=='linux'">
      <img
        src="@/assets/images/background.jpg"
        style="width: 100%;height: 100%;">
    </div>
    <!-- :style="loginBackground" -->
    <div
      class="login-page">
      <div class="login-label">
        <!-- @dblclick="clickShowNavbar" -->
        <img src="../assets/images/logo.png">
        <div
          class="switch-lang"
          :class="$i18n.locale == 'zh'?'active-zh':'active-en'">
          <span
            class="zh"
            @click="switchLang('zh')">中 文</span>
          | <span
            class="en"
            @click="switchLang('en')">English</span>
        </div>
        <div
          class="setting-button"
          @click="showSettingDialog">
          <i class="el-icon-setting" /> {{ $t('resource.setting') }}
        </div>
        <div
          class="clear-button"
          @click="clearStorage">
          <i class="el-icon-delete" /> {{ $t('resource.clearCache') }}
        </div>
      </div>
      <div style="position:relative;">
        <div class="opacity-label" />
        <div class="login-form">
          <div
            class="login-title"
            :style="$i18n.locale == 'zh'?'padding-left: 25%;':''">
            {{ $t('version.title') }}
          </div>
          <div class="login-content">
            <el-form
              ref="loginform"
              :rules="rules"
              :model="loginform"
              label-width="130px"
              @keydown.enter.native="confirmLogin">
              <el-form-item
                :label="$t('resource.curCluseter')"
                prop="cluster">
                <el-input
                  id="login_cluster"
                  v-model="loginform.cluster"
                  placeholder="10.10.10.10" />
              </el-form-item>
              <el-form-item
                :label="$t('login.tenantName')"
                prop="tenant">
                <el-input
                  id="login_tenant"
                  v-model="loginform.tenant" />
              </el-form-item>
              <el-form-item
                :label="$t('login.userName')"
                prop="username">
                <el-input v-model="loginform.username" />
              </el-form-item>
              <el-form-item
                :label="$t('login.pwd')"
                prop="userpwd">
                <el-input
                  v-model="loginform.userpwd"
                  show-password />
              </el-form-item>
              <span class="error-tip-cont">{{ error_Tips }}</span>
              <div>
                <div class="login-type">
                  <el-checkbox v-model="isLdapLogin" />
                  <span style="margin-right: 20px;">{{ $t('resource.ldap') }}</span>
                  <el-checkbox v-model="isRemember" />
                  <span>{{ $t('resource.remember') }}</span>
                </div>
                <div style="float:right;">
                  <el-button
                    id="confirm_login"
                    class="login-button"
                    @click.native.prevent="confirmLogin('loginform')">
                    {{ $t('resource.signin') }}
                  </el-button>
                </div>
                <div style="clear:both;" />
              </div>
            </el-form>
          </div>
        </div>
      </div>
      <div
        class="cluster-cont"
        :style="osType=='linux'?'':'background: linear-gradient(#21213f, #1c4587);'">
        <div class="cluster-title">
          {{ $t('resource.usedClusterTip') }}
        </div>
        <div class="cluster-labels">
          <div
            v-for="(item, index) in usedClusters"
            v-show="usedClusters"
            :key="index"
            class="lastest-cluster"
            :class="item == loginform.cluster ? 'active' : ''"
            @click="chooseCluster(item)">
            <i class="sg-iconfont im-icon-jiqun" />
            <p>{{ item }}</p>
          </div>
          <div
            v-show="!usedClusters"
            class="no-lastest-cluster">
            {{ $t('resource.noData') }}
          </div>
        </div>

        <div id="foot_cont" />
      </div>

      <system-setting-dialog
        v-if="showSetting"
        :show="showSetting"
        @closeSettingDialog="closeSettingDialog" />
    </div>
  </div>
</template>

<script>
import { mapState, mapMutations, mapActions } from 'vuex'
import Basic from '@/test/basic.js'
import { ipcRenderer } from 'electron'
import md5 from 'md5'
import { encryptAESPassword } from '@/components/common/js/aesPassword'
import systemSettingDialog from './common/systemSettingDialog'
import * as THREE from '@/components/common/js/three.module.js'
import ElectronStore from 'electron-store'
var store = new ElectronStore() // 存储数据
const exec = require('child_process').exec
var os = require('os')
export default {
  mixins: [Basic],
  components: {systemSettingDialog},
  data () {
    return {
      isLdapLogin: false,
      isRemember: false,
      showSetting: false,
      usedClusters: [],
      loadingHidden: '',
      osType: 'win',
      // valid_tenant: this.$t('login.tenantName'),
      // valid_user: this.$t('login.userName'),
      loginBackground: {
        // backgroundImage:
        // 'url(' + require('../assets/images/background.jpg') + ')',
        backgroundRepeat: 'no-repeat',
        backgroundSize: 'cover',
        height: '100%',
        width: '100%',
        color: '#fff'
      },
      loginform: {
        cluster: '',
        tenant: '',
        username: '',
        userpwd: ''
      },
      system_info: '',
      cur_cluster: '',
      port: '8080',
      protocolType: 'http', // 协议类型
      cur_ip: '', // 实际访问的地址信息
      error_Tips: '',
      login_click: 0,
      interval: null,
      clickNumTimer: '',
      clusterType: 1
    }
  },
  methods: {
    ...mapMutations('SystemConfig', ['setpwAESinfos', 'setHostInfos']),
    // clickShowNavbar () {
    //   ipcRenderer.send('openConsole')
    // },
    getFlow (idName, px, py, pz, range) {
      // var scene, camera, renderer, ribbon
      const container = document.querySelector(`#${idName}`)
      const init = () => {
        this.scene = new THREE.Scene()
        // 构造函数PerspectiveCamera
        // 该构造函数总共有四个参数，分别是fov，aspect，near，far 。
        // fov表示摄像机视锥体垂直视野角度，最小值为0，最大值为180，默认值为50，实际项目中一般都定义45，因为45最接近人正常睁眼角度；
        // aspect表示摄像机视锥体长宽比，默认长宽比为1，即表示看到的是正方形，实际项目中使用的是屏幕的宽高比；
        // near表示摄像机视锥体近端面，这个值默认为0.1，实际项目中都会设置为1；
        // far表示摄像机视锥体远端面，默认为2000，这个值可以是无限的，说的简单点就是我们视觉所能看到的最远距离。
        this.camera = new THREE.PerspectiveCamera(75, 1, 0.1, 1000)
        // this.camera.position.z = 2

        this.renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true })
        container.appendChild(this.renderer.domElement)

        // 材质对象Material
        // vertexShader(顶点着色器)/这个着色器允许你修改每一个传入的顶点的位置
        this.ribbon = new THREE.Mesh(
          new THREE.PlaneGeometry(1, 3, 128, 128),
          new THREE.ShaderMaterial({
            uniforms: {
              time: { value: 0.1 }
            },
            vertexShader: `
                varying vec3 vEC;
                uniform float time;

                float iqhash(float n) {
                  return fract(sin(n) * 43758.5453);
                }

                float noise(vec3 x) {
                  vec3 p = floor(x);
                  vec3 f = fract(x);
                  f = f * f * (3.0 - 2.0 * f);
                  float n = p.x + p.y * 57.0 + 113.0 * p.z;
                  return mix(mix(mix(iqhash(n), iqhash(n + 1.0), f.x),
                            mix(iqhash(n + 57.0), iqhash(n + 58.0), f.x), f.y),
                            mix(mix(iqhash(n + 113.0), iqhash(n + 114.0), f.x),
                            mix(iqhash(n + 170.0), iqhash(n + 171.0), f.x), f.y), f.z);
                }

                float xmb_noise2(vec3 x) {
                  return cos(x.z * 4.0) * cos(x.z + time / 10.0 + x.x);
                }

                void main() {
                  vec4 pos = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
                  vec3 v = vec3(pos.x, 0.0, pos.y);
                  vec3 v2 = v;
                  vec3 v3 = v;

                  v.y = xmb_noise2(v2) / 1.5; // 原本是8

                  v3.x -= time / 5.0;
                  v3.x /= 4.0;

                  v3.z -= time / 10.0;
                  v3.y -= time / 100.0;

                  v.z -= noise(v3 * 7.0) / 15.0;
                  v.y -= noise(v3 * 7.0) / 15.0 + cos(v.x * 2.0 - time / 2.0) / 1.0 + 0.1;
                  // v.y -= noise(v3 * 7.0) / 15.0 + cos(v.x * 2.0 - time / 2.0) / 5.0 - 0.3; 高度波动幅度
                  vEC = v;
                  gl_Position = vec4(v, 1.0);
                }
              `,
            fragmentShader: `
                uniform float time;
                varying vec3 vEC;

                void main()
                {
                  const vec3 up = vec3(0.0, 0.0, 1.0);
                  vec3 x = dFdx(vEC);
                  vec3 y = dFdy(vEC);
                  vec3 normal = normalize(cross(x, y));
                  float c = 1.0 - dot(normal, up);
                  c = (1.0 - cos(c * c)) / 1.0;
                  gl_FragColor = vec4(0,255,255, c * 2.8);
                }
              `,
            extensions: {
              derivatives: true,
              fragDepth: false,
              drawBuffers: false,
              shaderTextureLOD: false
            },
            side: THREE.DoubleSide,
            transparent: true,
            depthTest: false
          })
        )
        // 统一设置position中xyz坐标
        this.camera.position.set(px, py, pz)
        this.scene.add(this.ribbon)

        resize()
        window.addEventListener('resize', resize)
      }

      const resize = () => {
        const { offsetWidth, offsetHeight } = container
        this.renderer.setSize(offsetWidth, offsetHeight)
        this.renderer.setPixelRatio(devicePixelRatio)

        this.camera.aspect = offsetWidth / offsetHeight
        this.camera.updateProjectionMatrix()
        // this.ribbon.scale.set(50, 20, 20)

        this.ribbon.scale.set(this.camera.aspect * 1.55, 0.75, 1) // 变宽
      }

      const animate = () => {
        if (!this.ribbon) {
          return false
        }
        this.ribbon.material.uniforms.time.value += 0.02
        this.renderer.render(this.scene, this.camera)
        requestAnimationFrame(() => animate())
      }
      init()
      animate()
    },
    switchLang (lang) {
      // this.$refs.loginform.clearValidate()
      let switch_lang
      lang === 'zh' ? switch_lang = 'zh' : switch_lang = 'en'
      localStorage.setItem('language', switch_lang) // 后面会用做切换和将用户习惯存储到本地浏览器
      this.setState({
        attr: 'language',
        val: switch_lang
      })
      this.$i18n.locale = switch_lang
    },
    // 轻触缓存
    clearStorage () {
      sessionStorage.clear()
      localStorage.clear()
      store.delete('rememberMe')
      store.delete('isLdapLogin')
      store.delete('userInfo')
      this.usedClusters = []
      this.isRemember = false
      this.getHostInfos()
    },
    checkCluster (s) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${this.$t('validate.cluseterEmpty')}`))
        } else {
          const url = /^(?=^.{3,255}$)(http(s)?:\/\/)?(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
          const onlyIp = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
          const noHttp = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\:(\[0-9]|[1-9]\d{1,3}|[1-5]\d{4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-2]\d|6553[0-5])$/
          // 输入完整的http://xxxx:xxxx
          const urlIncPoot = /^(?=^.{3,255}$)(http(s)?:\/\/)?(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\:(\[0-9]|[1-9]\d{1,3}|[1-5]\d{4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-2]\d|6553[0-5])$/
          // const ipv6Re = /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/
          // const re = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;
          if (onlyIp.test(value) || urlIncPoot.test(value) || url.test(value) || noHttp.test(value)) {
            // 判断用户输入的url类型
            if (onlyIp.test(value)) {
              this.clusterType = 1
            } else if (url.test(value)) {
              this.clusterType = 2
            } else if (noHttp.test(value)) {
              this.clusterType = 3
            } else if (urlIncPoot.test(value)) {
              this.clusterType = 4
            }
            return callback()
          } else {
            return callback(new Error(`${this.$t('validate.cluseterFormat')}`))
          }
        }
      }
    },
    open (link) {
      this.$electron.shell.openExternal(link)
    },
    chooseCluster (val) {
      this.loginform.cluster = val
    },
    confirmLogin () {
      localStorage.removeItem('netSettingFlag')
      this.$refs.loginform.validate((valid) => {
        if (!valid) {
          return false
        }
        // 保存请求的url 访问连接都需要用它拼接接口
        if (this.cur_ip) {
          sessionStorage.setItem('url', this.cur_ip)
        } else {
          if (this.clusterType == 1) {
            // 只输入IP，默认http请求 端口8080
            this.cur_cluster = 'http://' + this.loginform.cluster + ':8080'
          }
          if (this.clusterType == 2) {
            // 没输入端口号
            this.cur_cluster = this.loginform.cluster + ':8080'
          }
          if (this.clusterType == 3) {
            // 没输入请求协议
            this.cur_cluster = 'http://' + this.loginform.cluster
          }
          if (this.clusterType == 4) {
            this.cur_cluster = this.loginform.cluster
          }
          sessionStorage.setItem('url', this.cur_cluster)
          // 获取端口号
          this.port = this.cur_cluster.substr(this.cur_cluster.lastIndexOf('\:') + 1, this.cur_cluster.length)
          // 获取协议
          this.protocolType = this.cur_cluster.substr(0, this.cur_cluster.indexOf(':'))
        }
        this.$store.commit('updateMsg')

        // 记住密码 暂定
        if (this.isRemember) {
          // if (this.osType == 'win') {
          store.set('rememberMe', true)// linux系统存在localStorage保存失败的问题
          store.set('isLdapLogin', this.isLdapLogin)
          store.set('userInfo', JSON.stringify(this.loginform))
          localStorage.setItem('rememberMe', true)
          localStorage.setItem('isLdapLogin', this.isLdapLogin)
          localStorage.setItem('userInfo', JSON.stringify(this.loginform))
        } else {
          store.delete('rememberMe')
          store.delete('isLdapLogin')
          store.delete('userInfo')
          localStorage.removeItem('rememberMe')
          localStorage.removeItem('isLdapLogin')
          localStorage.removeItem('userInfo')
        }

        // 存集群 便于读取
        let list = []
        if (localStorage.getItem('clusters')) {
          // 如果已有集群 填充
          list = JSON.parse(localStorage.getItem('clusters')) // 数组
          list.unshift(this.loginform.cluster)
          list = Array.from(new Set(list)) // 去重
        } else {
          list.push(this.loginform.cluster)
        }
        localStorage.setItem('clusters', JSON.stringify(list))
        this.getIp()
      })
    },
    getIp () {
      // eslint-disable-next-line standard/object-curly-even-spacing
      this.loadingHidden = this.$loading({ target: '.login-page'})
      localStorage.setItem('noErrorTipFlag', true)
      this.$Api.system.netInfos().then(res => {
        localStorage.removeItem('noErrorTipFlag')
        if (res.Ip) {
          sessionStorage.setItem('cur_hostIp', res.Ip)
          this.setState({
            attr: 'cur_hostIp',
            val: res.Ip
          })
        }
        this.getPwAES()
      }).catch(() => {
        this.loadingHidden.close()
        this.$message({
          message: this.$t('login.agentTip'),
          type: 'warning',
          duration: 3000
        })
        localStorage.removeItem('noErrorTipFlag')
      })
    },
    getPwAES () {
      // 密钥目前固定
      const data = {
        aes_key: '1~$c31kjtR^@@c2#',
        aes_vector: '#$3456$890A54321'
      }
      sessionStorage.setItem('aesConfigKey', data.aes_key)
      sessionStorage.setItem('aesConfigIv', data.aes_vector)
      this.setpwAESinfos(data)
      this.getLogin()
      // this.$Api.system.pwAESInfos().then(res => {
      //   if (res.scode == 0) {
      //     // self.aesConfig = res.data
      //     if (res.data.aes_key && res.data.aes_vector) {
      //       sessionStorage.setItem('aesConfigKey', res.data.aes_key)
      //       sessionStorage.setItem('aesConfigIv', res.data.aes_vector)
      //       this.setpwAESinfos(res.data)
      //     } else {
      //     }
      //   }
      // }).catch(() => {
      //   this.loadingHidden.close()
      // })
    },
    getLogin () {
      this.$nprogress.start()
      let pwd
      if (!this.isLdapLogin) {
        pwd = md5(this.loginform.userpwd)
      } else {
        pwd = encryptAESPassword(this.loginform.userpwd, this.aesConfig.aesIv, this.aesConfig.aesKey)
      }
      const params = {
        user: this.loginform.username,
        password: pwd,
        tenant: this.loginform.tenant,
        cluster_uuid: '',
        is_ldap: this.isLdapLogin,
        vdc_agent: this.hostId,
        vdc_login_ip: this.cur_hostIp
      }
      if (this.loginform.tenant === 'administrator') {
        localStorage.setItem('loginType', 'admin')
        params.tenant = 'system'
      } else if (this.loginform.tenant === this.loginform.username) {
        localStorage.setItem('loginType', 'tenant')
      } else {
        localStorage.setItem('loginType', 'user')
      }
      this.$Api.userOperation.userLogin(params).then((response) => {
        if (response.scode !== 0) {
          this.$nprogress.done()
          this.loadingHidden.close()
          if (response.scode === 10179) {
            this.login_click = response.data.login_limit_punish_remain_time
            if (parseInt(this.login_click) > 0) {
              this.interval = setInterval(function () {
                this.error_Tips =
                  '连续登录失败,账户锁定至' + this.login_click + '秒后允许登录'
                this.login_click = this.login_click - 1
                if (parseInt(this.login_click) <= 0) {
                  clearInterval(this.interval)
                  this.interval = null
                  this.error_Tips = ''
                }
              }, 1000)
            }
          } else if (response.scode === 10187) {
            this.$message({
              message: response.message_cn,
              type: 'error',
              duration: 1500
            })
            // this.error_Tips = '用户验证失败, 剩余尝试次数' + response.data.remain_retry_times + '次'
          } else if (response.scode === 11160) {
            this.error_Tips = this.$t('login.pwdExpired')
          } else if (response.scode === 11161) {
            this.error_Tips = this.$t('login.pwdExpired')
            // this.error_Tips = '密码已经过期,重置密码已发送至您的用户邮箱'
          } else if (response.scode === 11057) {
          // 当前节点非管理集群主节点
            this.cur_ip = response.data.candidates[0] // 应该访问的ip
            // 看用输入的是什么端口 用于拼接url
            if (this.clusterType == 1) {
            // 只输入IP，默认http请求 端口8080
              this.cur_ip = 'http://' + this.cur_ip + ':8080'
            }
            if (this.clusterType == 2) {
            // 没输入端口号
              this.cur_ip = this.protocolType + '://' + this.cur_ip + ':8080'
            }
            if (this.clusterType == 3) {
            // 没输入请求协议
              this.cur_ip = 'http://' + this.cur_ip + ':' + this.port
            }
            if (this.clusterType == 4) {
            // 是完整的
              this.cur_ip = this.protocolType + '://' + this.cur_ip + ':' + this.port
            }
            this.$store.commit('setState', [{ attr: 'url', val: this.cur_ip }])
            this.confirmLogin()
          } else {
            this.$message({
              message: response.message_cn,
              type: 'error',
              duration: 2500
            })
          }
        } else {
          // 登录成功
          this.loginSuccessCallback(response)
          this.error_Tips = ''
        }
      }).catch(() => {
        this.$nprogress.done()
        this.loadingHidden.close()
      })
    },
    // 管理员登录成功回调函数
    loginSuccessCallback (response) {
      const data = response.data
      // 保存用户登录信息
      sessionStorage.setItem('username', data.user_name)
      sessionStorage.setItem('useruuid', data.user_uuid)
      sessionStorage.setItem('token', data.token)
      sessionStorage.setItem('isLDAP', JSON.stringify(data.enable_ldap))
      if (!data.is_deploy_mode) {
        if (data.tenant == 'system') {
          // 管理员
          sessionStorage.setItem('isadmin', data.tenant)
        } else {
          if (data.role == 'administrator') { // 租户
            sessionStorage.setItem('isTenantRole', data.role)
          }
          if (data.role == 'normalUser') { // 用户
            sessionStorage.setItem('isUserRole', data.role)
          }
        }
        let tenant = data.tenant == 'system' ? '' : data.tenant
        sessionStorage.setItem('cur_tenant', tenant)
      }
      // 更新vuex的数据
      this.$store.commit('updateMsg')
      this.getApiKey()
      this.system_info = data.system_member_list
      // this.getCurrentCluster()
    },
    getApiKey () {
      this.$Api.userOperation.apiKey({filter_user: this.useruuid}).then(res => {
        if (res.scode == 0) {
          if (res.data && res.data.list.length !== 0) {
            // 如果不为空,取第一个作为apiKey
            sessionStorage.setItem('ApiKey', res.data.list[0].uuid)
            sessionStorage.removeItem('token')
            this.$store.commit('updateMsg')

            this.getUserSetting()
            // this.getCurrentCluster()
          } else {
            // 如果没有apiKey去创建
            this.getCreateApiKey()
          }
        } else {
          this.$nprogress.done()
          this.loadingHidden.close()
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      }).catch(() => {
        this.$nprogress.done()
        this.loadingHidden.close()
      })
    },
    getCreateApiKey () {
      this.$Api.userOperation.createApiKey({tenant: this.cur_tenant}).then(res => {
        if (res.scode == 0) {
          // 如果不为空,取第一个作为apiKey
          sessionStorage.setItem('ApiKey', res.data.uuid)
          sessionStorage.removeItem('token')
          this.$store.commit('updateMsg')
          this.getUserSetting()
          // this.getCurrentCluster()
        } else {
          this.$nprogress.done()
          this.loadingHidden.close()
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      }).catch(() => {
        this.$nprogress.done()
        this.loadingHidden.close()
      })
    },
    getUserSetting () { // 获取用户的个性化设置，以便于进行操作
      this.$Api.system.userSettingInfo({user_uuid: this.useruuid}).then(res => {
        if (res.data.securityStrategy) {
          // 如果这个字段存在,表示可以调用安全策略的接口
        }
        if (res.data.uuid) {
          this.setState({
            attr: 'setting',
            val: res.data
          })
          // 检查个性化设置中是否有开启 自动打开第一台topviewer
          if (this.setting.enable_auto_open_vm) {
            sessionStorage.setItem('auto_open_vm', true)
          }
        }
        this.getCurrentCluster(res.data.securityStrategy)
      }).catch(() => {
        this.$nprogress.done()
        this.loadingHidden.close()
      })
    },
    getSecurityStrategy () {
      // 获取对客户机的安全策略设置
      this.$Api.system.securityStrategyLisi({cluster_uuid: this.cluster_uuid, filter_agent_id: this.hostId, filter_type: 3}).then(res => {
        if (res.data.infos && res.data.infos[0].device_access_policy.enabled) {
          // 如果有对该登录账号有个性化设置
          if (this.setting.uuid) {
            this.setState({
              attr: 'securitySetting',
              val: res.data.infos[0].device_access_policy
            })
          }
        }
      })
    },
    getCurrentCluster (val) {
      // 获取当前集群名称再进行跳转

      // 跳转前判断apikey是否存在
      if (!this.ApiKey) {
        // apikey是用户的身份识别 如果apikey是空 说明没有用户信息 提示重试
        this.$message({
          message: this.$t('login.apikeyIsNull'),
          type: 'warning',
          duration: 6000
        })
        return false
      }
      this.$Api.userOperation.clusterList().then((response) => {
        if (response.scode == 0 && response.data && response.data.list) {
          // 优先级  优先默认 健康和未停用的  再考虑健康 再考虑未停用 都没有则默认选择的最后一个
          const datalist = response.data.list
          // 1.先拿健康
          const is_health_list = datalist.filter(el => el.is_health)
          let isCanClusterItem = ''
          if (is_health_list.length) {
            // 2.再在健康中 拿没停用的
            isCanClusterItem = is_health_list.find(item => !item.is_stop)
            if (!isCanClusterItem) {
              isCanClusterItem = is_health_list[0]
            }
          } else {
            isCanClusterItem = datalist.find(el => !el.is_stop)
          }
          if (!isCanClusterItem) {
            isCanClusterItem = datalist[datalist.length - 1]
          }
          sessionStorage.setItem('cluster_uuid', isCanClusterItem.uuid)
          sessionStorage.setItem('clustername', isCanClusterItem.name)
          sessionStorage.setItem(
            'clusterList',
            JSON.stringify(datalist)
          )
          this.$store.commit('updateMsg')
          if (val) {
            // 这个字段来自获取个性化设置的接口
            // 如果这个字段存在,表示可以调用安全策略的接口
            this.getSecurityStrategy()
          }
          if (this.system_info && this.system_info.server_mode == 'topdc') {
            sessionStorage.setItem('systemMode', 'topdc')
          }
          // 根据是否展示导航 跳转到不同的页面
          // if (this.setting.show_cloud_desktop) {
          this.$router.replace({ path: '/desktopVms' })
          // } else {
          //   this.$router.replace({ path: '/computer' })
          // this.getSystemInfo()
          // this.getSocket()
        } else {
          this.$nprogress.done()
          this.loadingHidden.close()
          this.$message({
            message: response.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      }).catch(() => {
        this.$nprogress.done()
        this.loadingHidden.close()
      })
    },
    getSystemInfo () { // 获取系统信息,主要获取当前系统模式,z
      const p = {
        cluster_uuid: this.cluster_uuid,
        alias_source: this.cur_cluster.substring(this.cur_cluster.indexOf('//') + 2, this.cur_cluster.lastIndexOf(':'))
      }
      this.$Api.system.systemInfo(p).then((res) => {
        this.$nprogress.done()
        this.loadingHidden.close()
        if (res.scode == 0) {
          if (res.data.server_mode == 'topdc') {
            sessionStorage.setItem('systemMode', 'topdc')
          }
          // 根据是否展示导航 跳转到不同的页面
          // if (this.setting.show_cloud_desktop) {
          this.$router.replace({ path: '/desktopVms' })
          // } else {
          //   this.$router.replace({ path: '/computer' })
          // }
        } else {
          this.$nprogress.done()
          this.loadingHidden.close()
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    // 展示设置
    showSettingDialog () {
      this.showSetting = true
    },
    closeSettingDialog (val) {
      this.showSetting = false
    },
    getHostInfos () {
      localStorage.setItem('noErrorTipFlag', true)
      this.$Api.system.hostMsg().then(res => {
        localStorage.removeItem('noErrorTipFlag')
        if (res.Status.code == 0) {
          // 存主机信息
          this.setHostInfos(res)
          sessionStorage.setItem('hostId', res.Name)
        } else {
          this.$message({
            message: res.Status.messageCn,
            type: 'warning',
            duration: 3000
          })
        }
      }).catch(() => {
        localStorage.removeItem('noErrorTipFlag')
        this.startAgent()
        // this.$message({
        //   message: this.$t('login.agentTip'),
        //   type: 'warning',
        //   duration: 3000
        // })
      })
    },
    initFlow () {
      this.scene = null
      this.camera = null
      this.renderer = null
      this.ribbon = null
    },
    startAgent () { // 开启agent
      let cmdline
      if (this.osType == 'win') {
        cmdline = '"C:\\Program Files\\TopViewer1.0\\vdiagent.exe"'
      } else {
        cmdline = '/usr/local/bin/vdiagent topmonitor start --conf /var/lib/vdiagent/vdiagent.conf'
      }
      exec(cmdline, {encoding: 'binary'}, (error, stdout, stderr) => {
        if (error) {
          // console.log('返沪信息', iconv.decode(error, 'cp936'))
        }
        console.log('拉起agent', '【error】', error, '【stdout】', stdout, '【stderr】', stderr)
      })
    }
  },
  computed: {
    ...mapState('SystemConfig', ['aesConfig', 'hostId']),

    rules () {
      return {
        cluster: [
          {
            required: true,
            validator: this.checkCluster(this.$t('resource.curCluseter')),
            trigger: 'change'
          }
        ],
        tenant: [{
          required: true,
          validator: this.checkBasicName(this.$t('login.tenantName')),
          trigger: 'change'
        }],
        username: [
          {
            required: true,
            validator: this.checkBasicName(this.$t('login.userName')),
            trigger: 'change'
          }
        ],
        userpwd: [{ required: true, message: this.$t('login.password'), trigger: 'change' }]

      }
    }
  },
  mounted () {
    this.initFlow()
    this.$nextTick(() => {
      if (os.platform().includes('win')) {
        this.getFlow('foot_cont', 0, 0.5, 20, 2.0)
      } else {
        this.osType = 'linux'
      }
    })
    // 展示前三个
    this.usedClusters = localStorage.getItem('clusters') && JSON.parse(localStorage.getItem('clusters')).splice(0, 3)
    // 获取主机信息
    this.getHostInfos()
    // 是否记住密码
    if (this.osType == 'win') {
      if (localStorage.getItem('rememberMe')) {
        this.isRemember = true
        this.isLdapLogin = localStorage.getItem('isLdapLogin') == 'true'
        this.loginform = JSON.parse(localStorage.getItem('userInfo'))
      }
    } else {
      if (store.get('rememberMe')) {
        this.isRemember = true
        this.isLdapLogin = store.get('isLdapLogin') == 'true'
        this.loginform = JSON.parse(store.get('userInfo'))
      }
    }
    // 重置菜单
    this.setState({
      attr: 'setting',
      val: { show_cloud_desktop: true, show_cloud_server: true, show_cloud_storage: true }
    })
    // 自动登录
    if (localStorage.getItem('autoLogin') || store.get('autoLogin')) {
      setTimeout(() => {
        this.confirmLogin('loginform')
      }, 5000)
    }
  },
  beforeDestroy () { // 切换时释放内存
    this.initFlow()
  }
}
</script>
<style>
/* body {
  font-family: PingFang SC, -apple-system, BlinkMacSystemFont, Segoe UI,
    Hiragino Sans GB, Microsoft YaHei, Helvetica Neue, Helvetica, Arial,
    sans-serif, Apple Color Emoji, Segoe UI Emoji, Segoe UI Symbol;
} */
i {
  font-style: inherit;
}
ul {
  padding-left: 0px;
}
li {
  list-style: none;
}
.el-loading-mask {
  background-color: aliceblue;
}
.el-loading-text {
  color: #3583e3;
}
</style>
<style lang="scss" scoped>
.login-page{
  // background-color: #21213f;
  height: 100%;color:#fff;
     position: absolute;
    top: 0;
    width: 100%;

}
.login-background {
  // background-image: url('~@/assets/images/background.jpg');
  background-repeat: no-repeat;
  background-size: cover; //background-size: auto 100%;
}
.el-input__icon{
  line-height: 30px !important;
}
.login-label {
  height: 25%;
  background-color: #21213f;
  img {
    height: 50px;
    margin-top: 50px;
    margin-left: 50px;
  }
}
.switch-lang{
    position: absolute;
    right: 20px;
    top: 20px;
    span{
      cursor: pointer;
    }
}
.active-zh{
  .zh{color: #338fc7;}
  .en{color: white;}
}
.active-en{
  .en{color: #338fc7;}
  .zh{color: white;}
}
.setting-button{
    cursor: pointer;
    position: absolute;
    top: 20px;
    right: 130px;
}
.clear-button{
    cursor: pointer;
    position: absolute;
    top: 20px;
    right: 220px;

}
.opacity-label {
  background-color: #27274e;
  color: white;
  height: 400px;
  opacity: 0.9;
}
.login-form {
  position: absolute;
  left: 47%;
  top: 50%;
  transform: translate(-50%, -50%);
  color: white;
  .login-title {
    font-size: 36px;
    padding-top: 37px;
    padding-bottom: 37px;
    text-align: center;
    opacity: 1;
  }
  .login-content {
    opacity: 1;
    /deep/.el-form-item__label {
      color: white;
      font-size: 15px;
    }
    /deep/.el-form-item {
      margin-bottom: 20px;
    }
    /deep/ .el-form-item__label::before{
      color:#f56c6c !important;
    }
    width: 380px;
    padding-bottom: 36px;
    margin: 0 auto;
  }
}
.login-button {
  width: 68px;
  height: 32px;
  color: white;
  border: #338fc7;
  background-color: #338fc7;
  cursor: pointer;
}
.cluster-cont{
  height: calc(100% - 25% - 400px);    position: relative;

}
.cluster-title {
  height: 45px;
  font-size: 16px;
  text-align: center;
  line-height: 45px;    padding-top: 2%;
}
.cluster-labels {
  width: 480px;
  height: 155px;
  border-radius: 6px;
  border: 1px solid rgba(196, 196, 196, 1);
  margin: auto;
  left: 0;    z-index: 2;
  position: absolute;
  right: 0;
  // margin: 0 auto;
  .lastest-cluster {
    display: inline-block;
    width: 33%;
    text-align: center;
    padding-top: 20px;
    i {
      font-size: 75px;
    }
  }
  .lastest-cluster:hover {
    color: #338fc7;
    cursor: pointer;
  }
}
.error-tip-cont{
    position: absolute;
    right: 0px;
    bottom: 5px;
    color: #f56c6c;
}
/deep/.el-form-item__error{
    color: #f56c6c !important;

}
.no-lastest-cluster{
    text-align: center;
    line-height: 130px;
}
.active {
  color: #338fc7;
}
.login-type{
margin-left: 85px;    display: inline-block;    padding-top: 5px;
.el-checkbox{
  margin-right: 5px;
}
}
#foot_cont{
    position: absolute;
    bottom: 0;
    width: 100%;height:100%;
}
</style>

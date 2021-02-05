// /* eslint-disable */
import Vue from 'vue'
import axiosInfo from './components/common/js/axois.js'

import App from './App'
import router from './router'
import store from './store'
import md5 from 'md5'
import NProgress from 'nprogress'
import 'nprogress/nprogress.css'

import MixinCommon from './mixins/common.js' // 全局mixin

// 国际化
import VueI18n from 'vue-i18n'
import en from './lang/en'
import zh from './lang/zh'
import locale from 'element-ui/lib/locale'
import elementEnLocale from 'element-ui/lib/locale/lang/en'
import elementZhLocale from 'element-ui/lib/locale/lang/zh-CN'

// import '../../static/js/aws-sdk-2.186.0.min.js'
// 封装API
import Api from './api'

// element ui 组件
import element from 'element-ui'
import '../../static/css/element/index.css'
// import '@/assets/css/iconfonts.scss'
import './assets/singleIconFont/iconfont.css'
// import 'xterm/dist/xterm.css'
import './assets/css/index.scss'
import './assets/css/optiscroll.scss'

// 工具库
import Util from './assets/utils/common'

// 自定义组件
import VxBox from './components/common/VxBox'
import VxScroll from './components/common/VxScroll'

// 获取端口号
let getFreePort = require('get-port-electron')
Vue.prototype.getFreePort = getFreePort

if (!process.env.IS_WEB) Vue.use(require('vue-electron'))
Vue.config.productionTip = false

window.md5 = md5
window.Util = Util

Vue.prototype.$nprogress = NProgress

NProgress.configure({
  showSpinner: false
})

Vue.use(VueI18n)
Vue.use(element)
Vue.use(VxBox)
Vue.use(VxScroll)

Vue.prototype.$Api = Api
Vue.mixin(MixinCommon)

// Vue.mixin(axiosInfo)

Vue.prototype.$elMessage = element.Message
const i18n = new VueI18n({
  locale: localStorage.getItem('language') ? localStorage.getItem('language') : 'zh',
  messages: {
    en: {
      ...en,
      ...elementEnLocale
    },
    zh: {
      ...zh,
      ...elementZhLocale
    }
  }
})
locale.i18n((key, value) => i18n.t(key, value))

new Vue({
  router,
  store,
  i18n,
  components: {
    App
  },
  template: '<App/>'
}).$mount('#app') // eslint-disable-line

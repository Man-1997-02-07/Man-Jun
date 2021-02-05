
import axios from 'axios'
import Vue from 'vue'
import element from 'element-ui'
import NProgress from 'nprogress'
import store from '@/store'
// loading 效果
let loadingRequestCount = 0
let loadingInstance
const showLoading = () => {
  if (loadingRequestCount === 0) {
    // element的服务方式 target 我这边取的是表格 项目是后台系统 每个页面都有表格 类似整个表格loading
    // 和在表格配置v-loading一样的效果，这么做是全局实现了，不用每个页面单独去v-loading
    // /* eslint-disable */
    // loadingInstance = element.Loading.service({ target: '.router-cont'})
  }
  loadingRequestCount++
}
const closeLoading = () => {
  if (loadingRequestCount <= 0) return
  loadingRequestCount--
  if (loadingRequestCount === 0) {
    // $nextTick(() => { // 以服务的方式调用的 Loading 需要异步关闭
    loadingInstance.close()
    // })
  }
}
// axios.defaults.timeout = 25000 // 设置超时 25秒
axios.interceptors.request.use(
  config => {
    // 设置访问路径，有些接口需要访问本地
    if (store.state.url) {
      if (config.url.includes('change_watermark')) {
        config.url = 'http://' + store.state.cur_hostIp + config.url
      } else if (config.url.includes('/top-viewer/notify') || config.url.includes('/top-viewer/resize') || config.url.includes('/top-viewer/disinput') || config.url.includes('/top-viewer/change_image_compression') || config.url.includes('/top-viewer/change_video_code')) {
        config.url = 'http://' + store.state.cur_hostIp + config.url
      } else if (config.url.includes('/MultipleBucketObject') || config.url.includes('/DownloadMultipleBucketObject')) {
        config.url = config.url
      } else if (config.url !== '/v1/vdiagent/uac' && config.url !== '/v1/vdiagent/upgrade' && config.url !== '/v1/vdiagent/topview/start' && config.url !== '/v1/vdiagent/topview/stop' && config.url !== '/v1/vdiagent/resource' && config.url !== '/vdiagent/poweroff' && config.url !== '/v1/vdiagent/topview/start' && config.url !== '/vdiagent/ping' && config.url !== '/vdiagent/interface' && config.url !== '/vdiagent/resolution' && config.url !== '/vdiagent/device' && config.url !== '/vdiagent/reboot') {
        config.url = store.state.url + '/v1' + config.url
      }
    }
    if (config.url == '/v1/vdiagent/uac' || config.url == '/v1/vdiagent/upgrade' || config.url == '/v1/vdiagent/topview/start' || config.url == '/v1/vdiagent/topview/stop' || config.url == '/v1/vdiagent/resource' || config.url == '/vdiagent/reboot' || config.url == '/vdiagent/poweroff' || config.url == '/vdiagent/ping' || config.url == '/vdiagent/interface' || config.url == '/vdiagent/resolution' || config.url == '/vdiagent/device') {
      config.url = 'http://127.0.0.1:9888' + config.url
    }
    // 多数请求头必带的
    if (store.state.token) {
      config.headers.Authorization = `Bearer ${store.state.token || ''}`
    }
    if (store.state.ApiKey) {
      config.headers.ApiKey = `${store.state.ApiKey}`
    }
    if (store.state.cur_hostIp) {
      config.headers.Remote_addr = `${store.state.cur_hostIp}`
    }
    // showLoading(config)
    return config
  },
  error => {
    // closeLoading()
    return Promise.reject(error)
  }
)

// 添加响应拦截器
axios.interceptors.response.use(
  response => {
    NProgress.done()
    // closeLoading()
    if (store.state.language == 'en') {
      response.data.message_cn = (response.data.message || response.data.message == '') ? response.data.message : (response.data.Status && response.data.Status.messageCn) ? response.data.Status.messageCn : ''
    }
    return response.data
  },
  error => {
    // closeLoading()
    // eslint-disable-next-line standard/object-curly-even-spacing
    // element.Loading.service({ target: '.login-page'}).close()
    const {response} = error
    NProgress.done()
    if (response && response.status) {
      switch (response.status) {
        case 404:
          if (!response.config.url.includes('/compute/desktop/pool/exclusive/start/vm')) {
            element.Message({
              message: store.state.language == 'zh' ? '请求不存在！' : 'Request does not exist',
              type: 'error',
              duration: 2000
            })
          }
          break
        case 500:
          element.Message({
            message: store.state.language == 'zh' ? '服务端错误！' : 'Service error',
            type: 'error',
            duration: 2000
          })
          break
        case 507:
          element.Message({
            message: store.state.language == 'zh' ? '存储空间不足' : 'Storage space is not enough',
            type: 'error',
            duration: 2000
          })
          break
      }
    } else {
      if (localStorage.getItem('netSettingFlag')) {
        return false
      }
      if (localStorage.getItem('useManagerRoleFlag')) {
        // 获取管理员权限接口会拒绝连接，但不提示错误信息
        localStorage.removeItem('useManagerRoleFlag')
        return false
      }
      // 升级的时候要测试agent是否启了，但不提示错误信息
      if (!localStorage.getItem('noErrorTipFlag')) {
        element.Message({
          message: store.state.language == 'zh' ? '请求失败！请检查网络或重试' : 'Request failed!Please check the network or try again',
          type: 'error',
          duration: 3500
        })
      }
    }
    return Promise.reject(error)
  }
)

Vue.prototype.$axios = axios

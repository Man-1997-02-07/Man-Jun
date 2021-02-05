import Vue from 'vue'
import Vuex from 'vuex'

import { createPersistedState, createSharedMutations } from 'vuex-electron'

import modules from './modules'

Vue.use(Vuex)

const state = {
  language: localStorage.getItem('language') ? localStorage.getItem('language') : 'zh',
  socket: null,
  bindVms: [], // 绑定的可操作虚拟机
  isKickedOut: {
    msg: '',
    value: false
  },
  optNoticeDialog: {// 通知客户端要关机/重启的操作
    isShow: false,
    optType: '',
    info: '',
    time: 300, // 执行倒数秒数
    exeFlag: true // 是否执行
  },
  isOpenNoticeDialog: false,
  noticeMsg: {
    type: '',
    msg: ''
  },
  setting: {// 用户的个性化设置
    show_cloud_desktop: true,
    show_cloud_server: true,
    show_cloud_storage: true,
    hide_navigation_bar: false// 隐藏导航栏
  },
  onLoading: false,
  search_val: '', // 公共搜索栏
  poolSetting: '', // 池的个性化设置
  securitySetting: '', // 安全策略的设置
  searchKeyWord: '',
  innerLoading: false,
  reFresh_cur_page: '',
  loadingHidden: '',
  // httpType: 1, // 1是http
  cur_guide_item: {},
  cur_hostIp: sessionStorage.getItem('cur_hostIp') || '',
  url: sessionStorage.getItem('url'),
  // urlWithoutPost: sessionStorage.getItem('urlWithoutPost'),
  username: sessionStorage.getItem('username'),
  useruuid: sessionStorage.getItem('useruuid'),
  cur_tenant: sessionStorage.getItem('cur_tenant'),
  token: sessionStorage.getItem('token'),
  clustername: sessionStorage.getItem('clustername'),
  cluster_uuid: sessionStorage.getItem('cluster_uuid'),
  isSuperAdmin: sessionStorage.getItem('isSuperAdmin') === 'systemadministrator', // 是否为超级管理员
  isadmin: (sessionStorage.getItem('isadmin') == 'system'), // 是否为管理员
  istenant: !!((sessionStorage.getItem('isTenant') != 'system' && sessionStorage.getItem('isTenantRole') == 'administrator')), // 是否为租户
  isuser: !!((sessionStorage.getItem('isTenant') != 'system' && sessionStorage.getItem('isUserRole') == 'normalUser')), // 是否为用户
  netDisk: (sessionStorage.getItem('netDisk') ? JSON.parse(sessionStorage.getItem('netDisk')) : []),
  ApiKey: sessionStorage.getItem('ApiKey'),
  enable_ldap: (parseInt(sessionStorage.getItem('isLDAP')) === 1)

}

const mutations = {
  // 更新信息
  updateMsg (state) {
    state.searchKeyWord = ''
    state.search_val = ''
    state.cur_guide_item = {}
    state.cur_hostIp = sessionStorage.getItem('cur_hostIp') || ''
    state.url = sessionStorage.getItem('url')
    state.username = sessionStorage.getItem('username') || ''
    state.useruuid = sessionStorage.getItem('useruuid') || ''
    state.cur_tenant = sessionStorage.getItem('cur_tenant') || ''
    state.token = sessionStorage.getItem('token') || ''
    state.ApiKey = sessionStorage.getItem('ApiKey') || ''
    state.clustername = sessionStorage.getItem('clustername') || ''
    state.cluster_uuid = sessionStorage.getItem('cluster_uuid') || ''
    state.isadmin = (sessionStorage.getItem('isadmin') == 'system')
    state.isSuperAdmin = (sessionStorage.getItem('isSuperAdmin') == 'systemadministrator')
    state.istenant = sessionStorage.getItem('isTenantRole') == 'administrator'
    state.isuser = sessionStorage.getItem('isUserRole') == 'normalUser'
    state.netDisk = (sessionStorage.getItem('netDisk') ? JSON.parse(sessionStorage.getItem('netDisk')) : [])
    state.enable_ldap = (parseInt(sessionStorage.getItem('isLDAP')) == 1)
  },
  // 设置
  setState (state, options) {
    if (Object.prototype.toString.call(options) == '[object Object]') {
      state[options.attr] = options.val
    } else if (Object.prototype.toString.call(options) == '[object Array]') {
      if (options.length > 0) {
        options.forEach((item) => {
          state[item.attr] = item.val
        })
      }
    }
  },
  createSocket (state) {
    let type = state.url.substr(0, 5) == 'https' ? 'wss' : 'ws'
    let ip = type == 'wss' ? state.url.substr(8, state.url.length) : state.url.substr(7, state.url.length)
    let wsurl = `${type}://${ip}/v1/user/vdc_agent/connection`
    state.socket = new WebSocket(wsurl)
  },
  closeSocket (state) {
    if (state.socket !== null) {
      // state.setting = { show_cloud_desktop: true, show_cloud_server: true, show_cloud_storage: true, hide_navigation_bar: false }
      state.poolSetting = ''
      state.securitySetting = ''
      state.bindVms = []
      state.socket.close()
    }
  },
  searchIndex (state, data) {
    state.search_val = data
  }
}
export default new Vuex.Store({
  state,
  modules,
  // actions,
  mutations,
  plugins: [
    createPersistedState()
    // createSharedMutations()
    // vuex-electron 引入了一个用于多进程间共享 Vuex Store 的状态的插件。
    // 如果没有多进程交互的需求，完全可以不引入这个插件，引用了会导致vuex失效
  ],
  strict: process.env.NODE_ENV !== 'production'
})

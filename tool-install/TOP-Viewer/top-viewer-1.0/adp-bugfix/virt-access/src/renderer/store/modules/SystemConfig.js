export default {
  namespaced: true,
  state: {
    multipeSelectScollTop: 0, // 多重下拉选择时，弹框滚动条的滚高
    // 存主机信息
    hostId: sessionStorage.getItem('hostId') || '',
    hostOs: '',
    agentVersion: '',
    agentNameId: '',
    isAdminRoler: '', // 是否是管理员权限
    // 存加密信息
    aesConfig: {// AES加密配置
      aesKey: sessionStorage.getItem('aesConfigKey') || '',
      aesIv: sessionStorage.getItem('aesConfigIv') || ''
    }
  },
  getters: {

  },
  mutations: {
    setMultipeSelectScollTop (state, data) {
      state.multipeSelectScollTop = data.val
    },
    setHostInfos (state, data) {
      state.hostId = data.Name
      state.hostOs = data.OsType
      state.agentVersion = data.AgentVersion.toString()
      state.agentNameId = data.Name
    },
    setpwAESinfos (state, data) {
      state.aesConfig.aesKey = data.aes_key
      state.aesConfig.aesIv = data.aes_vector
    }

  }
}

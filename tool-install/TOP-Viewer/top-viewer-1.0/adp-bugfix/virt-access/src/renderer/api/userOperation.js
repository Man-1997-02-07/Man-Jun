import axios from 'axios'
export default {
  clusterList (params) {
    return axios.get('/user/cluster/allow/list', {params})
  },
  // 用户登录
  userLogin (params) {
    return axios.post('/authentication/login', params)
  },
  // 用户登录
  userLogout (params) {
    return axios.post('/authentication/logout', params)
  },
  // 登录时先调这个
  vdcRegister (params) {
    return axios.post('/vdiagent/login', params)
  },
  // viewer与agent之间的接口定义
  // 设置vdc地址
  vdcAddress (params) {
    return axios.put('/agent/vdcset', params)
  },
  // 创建apiKey
  createApiKey (params) {
    return axios.post('/apikey/create', params)
  },
  // 获取apiKey
  apiKey (params) {
    return axios.get('/apikey/list', {params})
  }
}

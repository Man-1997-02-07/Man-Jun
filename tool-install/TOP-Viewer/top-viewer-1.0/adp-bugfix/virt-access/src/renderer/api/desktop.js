import axios from 'axios'
export default {
  // 云桌面列表获取
  desktopList (params) {
    return axios.get('/compute/desktop/pool/exclusive/list', {params})
  },
  // 云桌面详情
  desktopInfo (params) {
    return axios.get('/compute/desktop/pool/exclusive/inspect', {params})
  },
  // 云桌面下批量启动虚拟机
  desktopBatchStartVm (params) {
    return axios.post('/compute/desktop/pool/exclusive/batch/start/vm', params)
  },
  // 云桌面下启动单个虚拟机
  desktopStartVm (params) {
    return axios.post('/compute/desktop/pool/exclusive/start/vm', params)
  }
}

import axios from 'axios'
export default {
  // 虚拟机列表获取
  vmList (params) {
    return axios.get('/compute/list', {params})
  },
  // 虚拟机详情
  vmInfo (params) {
    return axios.get('/compute/inspect', {params})
  },
  // 虚拟机截屏
  computeScreenShot (params) {
    return axios.post('/compute/screenshot', params)
  },
  /**
   * 虚拟机vdc 控制台
   * @method computeLog
   * {
      "cluster_uuid":"",
      "tenant":"",
      "uuid":""        //虚拟机uuid
    }
   */
  computeVDCConsole (params) {
    return axios.post(`/compute/vdc/console`, params)
  },
  /// /桌面资源池中虚拟机注销还原(用户使用虚拟机前调用)
  checkVm (params) {
    return axios.post('/compute/desktop/pool/exclusive/vm/restore', params)
  },
  // 打开topview
  openTopview (params) {
    return axios.post('/v1/vdiagent/topview/start', params)
  },
  // 一键杀死topview拉起的remote-view
  closeAllVms (params) {
    return axios.post('/v1/vdiagent/topview/stop', params)
  },
  // 当用户自己退出登录或被踢下线时,用来清空普通用户桌面池里分配的虚拟机,防止占用资源;
  clearVms (params) {
    return axios.post('/compute/desktop/pool/exclusive/vm/cancel_assign/user', params)
  },
  ticketPrecesses (params) {
    return axios.get('/ticket/process/list', {params})
  },
  /**
     * 内置卷列表
     * @method volumeInternalList
     */
  volumeInternalList (params) {
    return axios.get('/volume/internal/list', { params })
  },

  /**
 * 获取共享卷列表
 * @method getShareVolumeList
		cluster_uuid
		filter_pool_uuid
		filter_name
		page_num
		page_size
  */
  getShareVolumeList (params) {
    return axios.post('/volume/share/list', params)
  },
  /**
   * 存储卷规格列表
   * @method specificationVolumeList
   */
  specificationVolumeList (params) {
    return axios.get('/specification/internalvolume/list', {
      params
    })
  },
  // 交换机列表
  switchList (params) {
    return axios.post('/network/switch/info/list', params)
  },
  /**
   * 内置池列表
   * @method poolInternalList
   */
  poolInternalList (params) {
    return axios.get('/pool/internal/list', { params })
  },
  /**
		* 统一提交的接口
		* @method POST:submit_ticket
		* @url:/ticket/submit
		* @params:'表单信息'
		*/
  submit_ticket (params) {
    return axios.post('/ticket/submit', params)
  },
  // 虚拟机操作
  // 虚拟机开机
  vmStart (params) {
    return axios.post('/compute/start', params)
  },
  // 关机
  vmShutoff (params) {
    return axios.post('/compute/shutoff', params)
  },
  // 安全关机
  vmSafeShutoff (params) {
    return axios.post('/compute/shutdown', params)
  },
  // 重启
  vmReboot (params) {
    return axios.post('/compute/reset', params)
  },
  // 安全重启
  vmSafeReboot (params) {
    return axios.post('/compute/reboot', params)
  },
  // 唤醒
  vmRestore (params) {
    return axios.post('/compute/restore', params)
  },
  // 恢复
  vmResume (params) {
    return axios.post('/compute/resume', params)
  },
  // 批量操作
  // 批量开机
  vmBatchStart (params) {
    return axios.post('/compute/batch_start', params)
  },
  // 批量重启
  vmBatchReboot (params) {
    return axios.post('/compute/batch_reset', params)
  },
  // 批量安全重启
  vmSafeBatchReboot (params) {
    return axios.post('/compute/batch_reboot', params)
  },
  // 批量关闭
  vmBatchShutoff (params) {
    return axios.post('/compute/batch_shutoff', params)
  },
  // 批量安全关闭
  vmSafeBatchShutoff (params) {
    return axios.post('/compute/batch_shutdown', params)
  },
  // 批量唤醒
  vmBatchRestore (params) {
    return axios.post('/compute/batch_restore', params)
  },
  // 批量恢复
  vmBatchResume (params) {
    return axios.post('/compute/batch_resume', params)
  }

}

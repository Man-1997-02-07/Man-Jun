import axios from 'axios'
export default {
  /**
	 * updateWsToken manager提供接口,保证ws不超时
	 */
  updateWsToken (params) {
    return axios.get('/system/dummy', {
      params
    })
  },
  hostMsg (params) {
    return axios.get('/vdiagent/device', {
      params
    })
  },
  hostUsedMsg (params) {
    return axios.get('/v1/vdiagent/resource', {
      params
    })
  },
  // 登录用户个性化设置详情
  userSettingInfo (params) {
    return axios.get('/compute/desktop/personalized_strategy/inspect/login_user', { params })
  },
  // 获取分辨率
  resolutionInfo (params) {
    return axios.get('/vdiagent/resolution', { params })
  },
  // 设置分辨率
  setResolution (params) {
    return axios.post('/vdiagent/resolution', params)
  },
  // 获取网络接口信息
  netInfos (params) {
    return axios.get('/vdiagent/interface', { params })
  },
  // 网络设置
  netSeeting (params) {
    return axios.post('/vdiagent/interface', params)
  },
  // 网络探测
  netTest (params) {
    return axios.post('/vdiagent/ping', params)
  },
  // agent重启
  agentReboot (params) {
    return axios.post('/vdiagent/reboot', params)
  },
  // agent关机
  agentPoweroff (params) {
    return axios.post('/vdiagent/poweroff', params)
  },
  // 升级安装包
  upgradePack (params) {
    return axios.post('/v1/vdiagent/upgrade', params)
  },
  // 以管理员权限运行agent
  runWithManager (params) {
    return axios.post('/v1/vdiagent/uac', params)
  },
  // 加密信息
  pwAESInfos (params) {
    return axios.get('/system/constant', {params})
  },
  // 修改分辨率
  // Request_body：{"resolution": 100}
  changeResolution (params) {
    return axios.get('/top-viewer/resize', {params})
  },
  // 禁止输入：
  // Request_body：{"disinput": "YES"("NO")}    YES：禁止输入；NO：允许输入
  changeInputMode (params) {
    return axios.get('/top-viewer/disinput', {params})
  },
  // 设置图像压缩算法：
  // Request_body: {"compression": "auto-glz"}    可选参数: auto-glz、auto-lz、quic、glz、lz、lz4、off
  setImageMode (params) {
    return axios.get('/top-viewer/change_image_compression', {params})
  },
  // 设置视频编码格式：
  // Request_body: {"videocode": "mjpeg"}  可选参数：mjpeg、vp8、vp9、h264
  setVideoMode (params) {
    return axios.get('/top-viewer/change_video_code', {params})
  },
  // 获取系统信息 此处主要拿系统模式
  systemInfo (params) {
    return axios.get('/system/member/list', {params})
  },
  /**
   * 获取系统配置详情(密码配置、ip白名单信息、集群配置参数)
   * @method settingInspect
   * @param {*} params
   * @url /system/setting/inspect
   */
  settingInspect (params) {
    return axios.get('/system/setting/inspect', { params })
  },
  /**
     * 获取用户详情
     * @param {string} tenant 租户ID
     * @param {string} user 用户ID
     */
  getUserInspect (params) {
    return axios.get('/user/inspect', {
      params
    })
  },
  /**
	 * 修改密码
	 * @method managerModifyPassword
	 * @param {String} user 用户名
	 * @param {String} old_password 旧密码
	 * @param {String} new_password 新密码
	 * @param {String} tenant 租户名
	 */
  managerModifyPassword (params) {
    return axios.post('/user/password/modify', params)
  },
  // 获取安全策略对客户机可能存在的设置信息
  // 如果存在设置,在唤起虚拟机时要
  securityStrategyLisi (params) {
    return axios.get('/vdc_agent/strategy/list', {params})
  }
}

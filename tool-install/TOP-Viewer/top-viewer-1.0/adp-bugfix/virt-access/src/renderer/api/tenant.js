import axios from 'axios'
export default {
  /**
  * 用户列表
  * @method usergrouplist
  * @param {String} tenant      租户名
  * @param {String} policy      策略名
    */
  usergrouplist (params) {
    return axios.get('/user/list', {
      params
    })
  },
  tenantQuota (params) {
    return axios.get('/tenant/quota/get', {
      params
    })
  }
}

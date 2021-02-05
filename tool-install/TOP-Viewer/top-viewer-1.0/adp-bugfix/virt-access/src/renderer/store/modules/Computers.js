export default {
  namespaced: true,
  state: {
    vmGuideList: [],
    vmList: [],
    cur_StartedVmList: [] // 已经开启的topView
  },
  mutations: {
    setGuideVmList (state, data) {
    },
    changeStartedVmList (state, data) {
      if (data.type == 'set') {
        state.cur_StartedVmList.push(data.data)
      } else if (data.type == 'delete') {
        state.cur_StartedVmList = state.cur_StartedVmList.filter(item => item.vmId !== data.data)
      } else if (data.type == 'close') {
        state.cur_StartedVmList = []
      } else if (data.type == 'eval') {
        state.cur_StartedVmList = data.data
      }
    }
  },
  actions: {
    // getVm
  }
}

export default {
  namespaced: true,
  state: {
    S3Obeject: { }
  },
  getters: {

  },
  mutations: {
    createS3Obeject (state, data) {
      if (!state.S3Obeject[data.key]) {
        state.S3Obeject[data.key] = {}
        state.S3Obeject[data.key].upload_fileList = []
        state.S3Obeject[data.key].reqList = []
      }
    },
    setUploadFileList (state, data) {
      state.S3Obeject[data.key].upload_fileList = data.upload_fileList.length ? data.upload_fileList : []
    },
    setReqList (state, data) {
      state.S3Obeject[data.key].reqList = data.reqList.length ? data.reqList : []
    }
  }
}

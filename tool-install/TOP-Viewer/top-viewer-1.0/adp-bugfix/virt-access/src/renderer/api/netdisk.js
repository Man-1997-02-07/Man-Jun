import axios from 'axios'
export default {
  /**
	 * 桶分配
	 * @method bucketAllot
	 * @param {Array-Object} user_list {user, user_name, tenant, bucket_count}
	 * @param {String} volume_uuid
	 */
  bucketAllot (params) {
    return axios.post('/user/bucket/assign', params)
  },
  // 获取S3存储卷信息
  /**
   * 内置卷挂载和还没挂载
   * 卷挂载
   * @method volumeMount
   * @param {String tenant}
   * @param {String volume_uuid}
   * @param {int page_num}
   * @param {int page_size}
   */
  volumeMount (params) {
    return axios.get('/volume/internal/mount/list', { params })
  },
  /**
	 * 获取普通用户被分配的S3卷
    * @method getS3VolumeList
    * @param {String} user
	  * @param {int} page_num
	  * @param {int} page_size
	  * @param {String} tenant
	  * @param {String} cluster_uuid
		*/
  getS3VolumeList (params) {
    return axios.get('/user/s3/list', {
      params
    })
  },
  /**
   * 获取下载S3卷的uuid
   * @param {*} params
   */
  downLoadBucketUuid (params) {
    return axios.post(`${params.s3Url}/v1/MultipleBucketObject`, params)
  },
  /**
   * 下载S3卷压缩后tar
   * @param {*} params
   */
  downLoadBucket (params) {
    return axios.post(`${params.s3Url}/v1/DownloadMultipleBucketObject`, params, {
      responseType: 'blob'
    })
  }
}

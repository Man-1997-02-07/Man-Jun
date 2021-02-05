import { mapState, mapMutations } from 'vuex'
import VsLoading from '@/components/common/vsLoading.vue'
export default {
  data () {
    return {
    }
  },
  components: {
    VsLoading
  },
  methods: {
    ...mapMutations([
      'setState', 'changeStartedVmList',
      'createSocket',
      'closeSocket',
      'updateMsg', 'searchIndex'
    ])
  },
  computed: {
    ...mapState([
      'language', 'setting', 'poolSetting', 'search_val', 'optNoticeDialog',
      'url', 'cur_hostIp', 'netDisk', 'isOpenNoticeDialog', 'noticeMsg', 'searchKeyWord',
      'innerLoading', 'reFresh_cur_page',
      'cluster_uuid',
      'clustername',
      'username', 'onLoading',
      'useruuid',
      'cur_tenant',
      'token', 'ApiKey',
      'isKickedOut',
      'socket',
      'isadmin',
      'istenant', 'bindVms',
      'isuser',
      'cur_guide_item', 'enable_ldap',
      'securitySetting'
      // , 'loadingHidden'
    ])
  }
}

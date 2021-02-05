<template>
  <el-dialog
    id="volume_netDisk--dialog-distribute"
    :title="$t('disk.assignUser')"
    :visible.sync="visible"
    :close-on-click-modal="false"
    :show-close="true"
    width="30%"
    @close="$emit('toggleDialogDistribute', false)">
    <div>
      <el-row
        :gutter="10"
        style="margin-bottom: 10px;">
        <el-col :span="20">
          <el-input
            id="volume_networkBox_filterName"
            v-model="filtersVal"
            :placeholder="$t('disk.enterName')"
            style="width:98%;vertical-align: middle;"
            class="selectType">
            <i
              v-if="filtersVal !== ''"
              id="volume_networkBox_clearName"
              slot="suffix"
              class="el-input__icon el-icon-error"
              style="position:absolute;top:0px;left:-20px;"
              @click="clearFilter" />
          </el-input>
        </el-col>
        <el-col
          :span="4">
          <el-button
            style="margin-left:5px;"
            @click="searchUser">
            <i class="el-icon-search" />{{ $t('disk.seach') }}
          </el-button>
        </el-col>
      </el-row>
      <el-table
        :data="allUserList"
        tooltip-effect="light">
        <el-table-column
          label="ID"
          align="center"
          type="index"
          width="80" />
        <el-table-column
          prop="user"
          :label="$t('disk.user')"
          width="200"
          :show-overflow-tooltip="true">
          <template
            slot="header"
            slot-scope="scope">
            <el-checkbox
              v-model="isAllSelect"
              :title="scope"
              @change="handleCheckAllChange">
              {{ $t('disk.user') }}
            </el-checkbox>
          </template>
          <template slot-scope="scope">
            <el-checkbox-group
              v-model="checkUsers">
              <el-checkbox :label="scope.row.name" />
            </el-checkbox-group>
          </template>
        </el-table-column>
        <el-table-column
          align="center"
          prop="setauthor"
          :label="$t('disk.authority')">
          <template slot-scope="scope">
            <el-radio-group v-model="scope.row.Permission">
              <el-radio label="READ">
                {{ $t('disk.readonly') }}
              </el-radio>
              <!-- <el-radio label="WRITE">
                只写
              </el-radio> -->
              <el-radio label="FULL_CONTROL">
                {{ $t('disk.readwrite') }}
              </el-radio>
            </el-radio-group>
          </template>
        </el-table-column>
      </el-table>
      <el-col
        :span="24"
        style="padding-bottom: 8px;">
        <el-pagination
          layout="total, prev, pager, next, jumper"
          :page-size="pageSize"
          :total="total"
          :current-page="pageNum+1"
          @current-change="handleCurrentChange" />
      </el-col>
    </div>
    <div slot="footer">
      <el-button
        type="primary"
        :disabled="innerLoading"
        @click="confirmDistribute">
        {{ $t('resource.confirm') }}
      </el-button>
      <el-button
        class="close-button"
        @click="$emit('toggleDialogDistribute', false)">
        {{ $t('resource.cancel') }}
      </el-button>
    </div>
    <vs-loading
      :is-show="innerLoading"
      class-name="vs-inner-loading" />
  </el-dialog>
</template>

<script>
export default {
  components: {

  },
  props: {
    dialogDistributeVisible: {
      type: Boolean,
      default: false
    },
    aclList: {
      type: Array
    },
    curClusterUuid: {
      type: String
    },
    tenant_uuid: {
      type: String
    },
    volume_uuid: {
      type: String
    }
  },
  data () {
    return {
      visible: this.dialogDistributeVisible,
      allUsers: [],
      allUserList: [],
      checkUsers: [],
      filtersVal: '',
      isAllSelect: false,
      volumeToUserList: [],
      total: 0,
      pageSize: 10,
      pageNum: 0
    }
  },
  watch: {
    dialogDistributeVisible (newval) {
      this.visible = newval
      if (newval) {
        this.allUsers = []
        this.allUserList = []
        this.checkUsers = []
        this.volumeToUserList = []
        this.getAllUsers()
      }
    },
    checkUsers (newval) {
      this.isAllSelect = this.allUserList.length === newval.length
    }
  },
  mounted () {

  },
  methods: {
    handleCurrentChange (val) {
      this.pageNum = val - 1
      this.getAllUsers()
    },
    confirmDistribute () {
      const self = this
      const list = []
      const noSelected = []
      self.allUserList.forEach(item => {
        let isSelect = false
        self.checkUsers.forEach(name => {
          if (name === item.name) {
            list.push(item)
            isSelect = true
          }
        })
        !isSelect && noSelected.push(item)
      })
      let initUsers = self.aclList
      noSelected.forEach(item => {
        initUsers.forEach(v => {
          if (item.uuid === v.uuid) {
            self.volumeToUserList.forEach(vo => {
              if (vo.user === v.uuid) {
                vo.bucket_count--
              }
            })
            v.uuid = ''
          }
        })
      })
      initUsers = initUsers.filter(item => !!item.uuid)
      list.forEach(item => {
        let isOwned = false
        initUsers.forEach(v => {
          if (item.uuid === v.uuid) {
            isOwned = true
            v.Permission = item.Permission
          }
        })
        if (!isOwned) {
          initUsers.push(item)
          isOwned = false
          self.volumeToUserList.forEach(v => {
            if (item.uuid === v.user) {
              isOwned = true
              v.bucket_count++
            }
          })
          if (!isOwned) {
            self.volumeToUserList.push({
              user: item.uuid,
              user_name: item.name,
              tenant: item.tenant,
              bucket_count: 1
            })
          }
        }
      })
      const params = {
        user_list: self.volumeToUserList,
        volume_uuid: self.volume_uuid,
        cluster_uuid: self.curClusterUuid
      }
      self.setState({
        attr: 'innerLoading',
        val: true
      })
      self.$Api.netdisk.bucketAllot(params).then(response => {
        self.setState({
          attr: 'innerLoading',
          val: false
        })
        if (response.scode == 0) {
          self.$emit('confirmDistribute', initUsers)
        } else {
          self.$message({
            message: response.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    getAllUsers () {
      const self = this
      const parmas = {
        cluster_uuid: self.cluster_uuid || self.curClusterUuid,
        tenant: self.cur_tenant || self.tenant_uuid,
        show_tenant_admin: false,
        page_num: self.pageNum,
        page_size: self.pageSize,
        filter_name: self.filtersVal.trim()
      }
      // if (self.allotForm.usertype === '本地用户') {
      //   parmas.filter_no_ldap = true
      // } else {
      //   parmas.filter_no_local = true
      // }
      self.setState({
        attr: 'innerLoading',
        val: true
      })
      self.$Api.tenant.usergrouplist(parmas).then((response) => {
        self.setState({
          attr: 'innerLoading',
          val: false
        })
        self.allUsers = []
        if (response.scode == 0) {
          const data = response.data
          self.volumeToUserList = []
          data.list.forEach(item => {
            item.Permission = 'READ'
            self.aclList.forEach(v => {
              if (item.uuid === v.uuid) {
                item.Permission = v.Permission
              }
            })
            if (item.s3_volumes && item.s3_volumes[self.volume_uuid]) {
              self.volumeToUserList.push({
                user: item.uuid,
                user_name: item.name,
                tenant: item.tenant,
                bucket_count: item.s3_volumes[self.volume_uuid].bucket_count
              })
            }
          })
          self.allUserList = data.list
          self.total = data.total_count
          self.checkUsers = []
          self.allUserList.forEach(item => {
            self.aclList.forEach(v => {
              if (item.uuid === v.uuid) {
                self.checkUsers.push(item.name)
              }
            })
          })
        } else {
          self.$message({
            message: response.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    handleCheckAllChange (val) {
      const self = this
      if (val) {
        self.checkUsers = []
        self.allUserList.forEach(item => {
          self.checkUsers.push(item.name)
        })
      } else {
        self.checkUsers = []
      }
    },
    clearFilter () {
      this.filtersVal = ''
      this.getAllUsers()
    },
    searchUser () {
      this.pageNum = 0
      this.getAllUsers()
    }
  },
  computed: {

  }
}
</script>

<style scoped lang="scss">
  ul{
    border:1px solid rgba(231,231,231,1);
    li{
      display: inline-block;
      box-sizing: border-box;
      width: 50%;
      padding-left: 10px;
      text-align: left;
      line-height: 35px;
      font-size: 13px;
      font-weight:400;
      color:rgba(50,49,50,1);
      border-bottom:1px solid rgba(231,231,231,1);
      &:nth-child(2n){
        border-right:1px solid rgba(231,231,231,1);
      }
      &:nth-child(2), &:nth-child(3){
        border-top: none;
      }
      &:nth-last-child(1),&:nth-last-child(2){
        border-bottom: none;
      }
      &:first-child{
        text-align: center;
        width: 100%;
        font-size: 14px;
      }
    }
    &.odd li:nth-last-child(2){
      border-bottom: 1px solid rgba(231,231,231,1);
    }
  }
  .page{
    padding-top: 5px;
    text-align: right;
  }
</style>

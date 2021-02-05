<template>
  <div>
    <el-dialog
      id="modify_user_pw_dialog"
      :title="$t('password.modifyPw')"
      :visible.sync="visible"
      :close-on-click-modal="false"
      :modal-append-to-body="false"
      width="460px"
      @close="$emit('update:show', false)">
      <el-form
        ref="addForm"
        :model="addForm"
        :rules="rules"
        :label-width="language=='zh'?'80px':'135px'"
        @keyup.enter.native="confirmModify('addForm')">
        <input
          type="password"
          autocomplete="new-password"
          style="position:absolute;top:-1000px;"
        >
        <el-form-item
          :label="$t('password.oldpasswd')"
          prop="oldPassword">
          <el-input
            v-if="visible"
            v-model="addForm.oldPassword"
            show-password
            :placeholder="$t('password.enterOldPasswdTip')"
            :maxlength="256" />
        </el-form-item>
        <!--请输入新密码-->
        <el-form-item
          :label="$t('password.newPasswd')"
          prop="newPassword">
          <el-input
            v-if="visible"
            v-model="addForm.newPassword"
            show-password
            :placeholder="$t('password.enterNewPasswdTip')"
            :maxlength="256"
          />
        </el-form-item>
        <!--确认密码-->
        <el-form-item
          :label="$t('password.confirmPasswd')"
          prop="confirmPassword">
          <el-input
            v-if="visible"
            v-model="addForm.confirmPassword"
            show-password
            :placeholder="$t('password.confirmPasswdTip')"
            :maxlength="256"
          />
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button
          id="common_modify_passwd"
          type="primary"
          :disabled="innerLoading"
          @click="confirmModify('addForm')">
          {{ $t('resource.confirm') }}
        </el-button>
        <el-button
          class="close-button"
          @click="visible = false">
          {{ $t('resource.cancel') }}
        </el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
export default {
  props: {
    show: {
      type: Boolean,
      default: false
    }
  },
  data () {
    const validatePass = (rule, value, callback) => {
      if (value === '') {
        callback(new Error(this.$t('password.pleaseConfirmPasswd')))
      } else if (value !== this.addForm.newPassword) {
        callback(new Error(this.$t('password.passwdIsDiffrent')))
      } else {
        callback()
      }
    }
    return {
      visible: this.show,
      curUserEmail: '',
      addForm: {
        oldPassword: '',
        newPassword: '',
        confirmPassword: ''
      },
      passwordConfig: {},
      rules: {
        oldPassword: [
          { required: true, message: this.$t('password.enterOldPasswdTip'), trigger: 'change' }
        ],
        newPassword: [
          { validator: this.passwordRules(), trigger: 'change' },
          { required: true, message: this.$t('password.passwdCannotEmpty'), trigger: 'change' }
        ],
        confirmPassword: [
          { required: true, validator: validatePass, trigger: 'change' }
        ]
      },
      lastPasswordModifyTime: null
    }
  },
  watch: {
    show (val) {
      this.visible = this.show
      if (val) {
        this.getTime()
        this.getPasswordConfig()
      }
      !val &&
        setTimeout(() => {
          this.$refs['addForm'].resetFields()
        }, 0)
    }
  },
  methods: {
    getTime () {
      const params = {
        user: sessionStorage.getItem('useruuid')
      }
      if (this.isadmin) {
        params.tenant = sessionStorage.getItem('isadmin')
      }
      if (this.istenant || this.isuser) {
        params.tenant = this.cur_tenant
      }
      this.$Api.system.getUserInspect(params).then(res => {
        if (res.scode === 0) {
          this.lastPasswordModifyTime = res.data.last_password_modify_time // 上一次修改时的时间
        } else {
          this.$message({
            message: res.message_cn,
            type: 'error',
            duration: 1500
          })
        }
      })
    },
    passwordRules () {
      const checkPassword = (rule, value, callback) => {
        if (!value) {
          // 密码不能为空
          return callback(new Error(this.$t('password.passwdCannotEmpty')))
        }
        const {
          password_minimum_length,
          password_must_include_special_char,
          password_special_char
        } = this.passwordConfig
        if (value.indexOf(' ') !== -1) {
          // 不能包含空格
          return callback(new Error(this.$t('password.passwdCannotIncludeBlankSpace')))
        }
        if (value.length < password_minimum_length) {
          return callback(new Error(`至少输入${password_minimum_length}个字符`))
        }
        if (password_must_include_special_char) {
          const charArray = password_special_char.split('')

          charArray.forEach((text, index) => {
            if (['-', '[', ']', '/'].indexOf(text) > -1) {
              charArray[index] = '\\' + text
            } else if (text === '\\') {
              charArray[index] = '\\\\'
            }
          })
          console.log('text', charArray.join(''))
          const re = new RegExp('[' + charArray.join('') + ']')
          // console.log('test', re.test(value))
          if (!re.test(value)) {
            // 密码至少含有一位特殊字符
            return callback(new Error(this.$t('password.SpecialCharTip')))
          } else {
            callback()
          }
        } else {
          if (value) {
            // 不能包含空格
            if (value.indexOf(' ') !== -1) {
              return callback(new Error(this.$t('password.passwdCannotIncludeBlankSpace')))
            } else {
              callback()
            }
          }
        }
      }
      return checkPassword
    },
    getPasswordConfig () {
      this.$Api.system.settingInspect().then(res => {
        if (res.scode == 0) {
          res.data.password_complexity_policy['password_special_char'] = res.data.password_special_char
          this.passwordConfig = res.data.password_complexity_policy
        }
      })
    },
    confirmModify (formName) {
      this.$refs[formName].validate(valid => {
        if (valid) {
          // 获取设置的数据
          let flag = 1
          this.$Api.system.settingInspect().then(res => {
            if (res.scode == 0) {
              // 请求成功了
              if (
                res.data.password_complexity_policy.password_minimal_modify_age
              ) {
                // 如果限制了修改密码的最小间隔时间
                const currentTime = new Date().getTime().toString().substr(0, 10)
                const minModifyTime = res.data.password_complexity_policy.password_minimal_modify_age
                const differenceTime = minModifyTime - (currentTime - this.lastPasswordModifyTime)
                if (differenceTime > 0) {
                  // 时间间隔不符合设置，不可修改
                  flag = 2
                  this.$message({
                    message:
                      '不满足二次修改密码最短时间间隔，请' +
                      this.parseTime(this.lastPasswordModifyTime + minModifyTime) +
                      '后再试',
                    type: 'error',
                    duration: 5000
                  })
                }
              }
            }
            if (flag == 1) {
              const params = {
                user: sessionStorage.getItem('useruuid'),
                old_password: md5(this.addForm.oldPassword),
                new_password: md5(this.addForm.newPassword),
                user_name: this.username,
                cluster_uuid: this.cluster_uuid
              }
              if (this.isadmin) {
                params.tenant = sessionStorage.getItem('isadmin')
              }
              if (this.istenant || this.isuser) {
                params.tenant = this.cur_tenant
              }
              this.$Api.system.managerModifyPassword(params).then(res => {
                if (res.scode == 0) {
                  this.$message({
                    message: '密码修改成功！',
                    type: 'success',
                    duration: 3000
                  })
                  this.$emit('update:show', false)
                  this.closeSocket()
                } else {
                  this.$message({
                    message: res.message_cn,
                    type: 'error',
                    duration: 3000
                  })
                }
              })
            }
          })
        }
      })
    },
    parseTime (time, cFormat) {
      if (arguments.length === 0) {
        return null
      }
      const format = cFormat || '{y}-{m}-{d} {h}:{i}:{s}'
      let date
      if (typeof time === 'object') {
        date = time
      } else {
        if ((typeof time === 'string') && (/^[0-9]+$/.test(time))) {
          time = parseInt(time)
        }
        if ((typeof time === 'number') && (time.toString().length === 10)) {
          time = time * 1000
        }
        date = new Date(time)
      }
      const formatObj = {
        y: date.getFullYear(),
        m: date.getMonth() + 1,
        d: date.getDate(),
        h: date.getHours(),
        i: date.getMinutes(),
        s: date.getSeconds(),
        a: date.getDay()
      }
      const time_str = format.replace(/{([ymdhisa])+}/g, (result, key) => {
        const value = formatObj[key]
        // Note: getDay() returns 0 on Sunday
        if (key === 'a') { return ['日', '一', '二', '三', '四', '五', '六'][value] }
        return value.toString().padStart(2, '0')
      })
      return time_str
    }
  }
}
</script>

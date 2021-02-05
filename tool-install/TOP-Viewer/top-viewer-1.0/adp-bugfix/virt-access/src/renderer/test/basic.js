var os = require('os')
export default {

  methods: {
    testBasic (s, arr) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}不能为空`))
        } else {
          const re = /[^\a-\z\A-\Z0-9\u4E00-\u9FA5\-\_]/g
          if (!re.test(value)) {
            // const regex = new RegExp("[\\u4E00-\\u9FFF]+","g");
            // if(regex.test(value)){
            //     return callback(new Error(`${s}不能含有中文`));
            // }else{
            if (value.length < 3 || value.length > 40) {
              return callback(new Error(`${s}由3~40位字符组成`))
            } else {
              if (Util.isArray(arr)) {
                let isKeywords = false
                arr.forEach(item => {
                  String(item).toUpperCase() === String(value).toUpperCase() && (isKeywords = true)
                })
                if (isKeywords) {
                  return callback(new Error(`${value}为系统保留关键字，不可使用`))
                } else {
                  callback()
                }
              } else {
                callback()
              }
            }
            // }
          } else {
            return callback(new Error(`${s}不能含特殊字符`))
          }
        }
      }
    },
    validateName (str, arr) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${str}不能为空`))
        } else {
          callback()
        }
      }
    },
    // 检验非负数
    testCount (s) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`不能为空`))
        } else {
          const re = /(^[1-9]\d*$)/
          if (re.test(value)) {
            return callback()
          } else {
            return callback(new Error(`请输入正整数`))
          }
        }
      }
    },
    // 输入单位的统一（统一是G和T 但是输入必须是0.5的整数倍）
    checkCapacity (s) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}不能为空`))
        } else {
          const re = /^[1-9]\d*([1-9]\d*\.\d{1})?$/
          if (re.test(value) || String(value) === '0.5') {
            if (value % 0.5 == 0) {
              return callback()
            } else {
              return callback(new Error(`${s}输入必须是0.5的整数倍`))
            }
          } else {
            return callback(new Error(`${s}输入不合法，请重新输入`))
          }
        }
      }
    },
    // 判断是否是Ipv6类型
    testIsIpv6Type (value) {
      return /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/.test(value)
    },
    checkCreatComputeNum (s) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}不能为空`))
        } else {
          const re = /^[1-9]{1}[0-9]*$/
          if (re.test(value)) {
            if (parseInt(value) <= 100) {
              return callback()
            } else {
              return callback(new Error(`${s}不能超过100`))
            }
          } else {
            return callback(new Error(`${s}输入错误！`))
          }
        }
      }
    },
    checkBasicName (s) {
      return (rule, value, callback) => {
        if (value) {
          var re = /[^\a-\z\A-\Z0-9\u4E00-\u9FA5\-\_]/g // eslint-disable-line
          if (re.test(value)) {
            callback(new Error(`${s}` + this.$t('validate.includeIllegalChar')))
          } else {
            if (String(value).length < 3 || String(value).length > 32) {
              callback(new Error(`${s}` + this.$t('validate.lengthLimit')))
            } else {
              callback()
            }
          }
        } else {
          callback(new Error(this.$t('validate.pleaseEnter') + `${s}`))
        }
      }
    },
    testAddress (s, options) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}` + this.$t('validate.empty')))
        } else {
          if (!os.platform().includes('win')) {
            // linux系统下不校验，允许输入中文
            return callback()
          } else {
            const ipv4Re = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
            const ipv6Re = /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/
            // const re = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/;
            if (ipv4Re.test(value) || ipv6Re.test(value)) {
              return callback()
            } else {
              return callback(new Error(`${s}` + this.$t('validate.errorFormat')))
            }
          }
        }
      }
    },
    // 检验子网掩码
    testMask (s, options) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}` + this.$t('validate.empty')))
        } else {
          const rep = /^(254|252|248|240|224|192|128|0)\.0\.0\.0|255\.(254|252|248|240|224|192|128|0)\.0\.0|255\.255\.(254|252|248|240|224|192|128|0)\.0|255\.255\.255\.(254|252|248|240|224|192|128|0)$/
          if (rep.test(value)) {
            return callback()
          } else {
            return callback(new Error(`${s}` + this.$t('validate.errorFormat')))
          }
        }
      }
    },
    // 检验网关掩码
    testGateway (s, options) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}` + this.$t('validate.empty')))
        } else {
          const rep = /^192\.168(\.(\d|([1-9]\d)|(1\d{2})|(2[0-4]\d)|(25[0-5]))){2}$/
          if (rep.test(value)) {
            return callback()
          } else {
            return callback(new Error(`${s}` + this.$t('validate.errorFormat')))
          }
        }
      }
    },
    // 检验 ip+端口号的 (例10.30.12.66:8080或 [2001:db8:8:1::50]:8080)
    testNetworkSegmentPort (s) {
      return (rule, value, callback) => {
        if (!value) {
          return callback(new Error(`${s}不能为空`))
        } else {
          const ipv4Re = /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
          const ipv6Re = /^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$/
          const isIpv6 = value.indexOf('[') > -1
          let ip = ''
          let port = ''
          try {
            if (isIpv6) {
              ip = (value.split(']')[0]).substring(1)
              port = (value.split(']')[1]).substring(1)
            } else {
              ip = value.split(':')[0]
              port = (value.split(':')[1])
            }
          } catch (e) {
            return callback(new Error(`${s}格式不正确`))
          }
          if ((ipv4Re.test(ip)) && value.indexOf(':') < 0) {
            callback()
          } else if (ipv6Re.test(ip) && value.indexOf(']:') < 0) {
            callback()
          } else if (ipv6Re.test(ip) && (port > 0)) {
            callback()
          } else if ((ipv4Re.test(ip)) && (port > 0)) {
            callback()
          } else {
            return callback(new Error(`${s}格式不正确`))
          }
        }
      }
    },

    testName (s, options) {
      let is_req = true
      let is_num = false
      let isSamber = false
      // console.log(options)
      if (Util.isObject(options)) {
        is_req = options.required === undefined ? true : options.required
      }
      if (Util.isObject(options)) {
        is_num = options.name !== '' ? true : (options.isNum === undefined ? false : options.isNum)
      }
      if (Util.isObject(options)) {
        isSamber = options.isSamber
      }
      return (rule, value, callback) => {
        let _this = this
        function test () {
          if (re.test(value)) {
            return callback(new Error(`${s + _this.$t('validate.special')}`))
          } else {
            if (value.length < 3 || value.length > 32) {
              return callback(new Error(`${s + _this.$t('validate.lengthLimit')}`))
            } else {
              var str = /^\d{1,}$/
              if (str.test(value) && isSamber) {
                return callback(new Error(`${s + _this.$t('validate.cannotOnlynNum')}`))
              } else {
                callback()
              }
            }
          }
        }
        if (!value) {
          if (is_req) {
            return callback(new Error(`${s + _this.$t('validate.empty')}`))
          } else {
            callback()
          }
        } else {
          var re = /[^\a-\z\A-\Z0-9\u4E00-\u9FA5\-\_]/g
          test()
        }
      }
    }
  }
}

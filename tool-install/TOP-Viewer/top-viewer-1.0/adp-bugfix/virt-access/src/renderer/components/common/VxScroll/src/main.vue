<template>
  <el-row
    :id="id"
    :class="['optiscroll',className]">
    <slot />
  </el-row>
</template>
<script>
import Optiscroll from 'optiscroll'
import { mapMutations } from 'vuex'
export default {
  name: 'VxScroll',
  props: {
    id: {
      type: String,
      default: function () {
        return `vx-scroll-${new Date().getTime()}-${Math.floor(Math.random() * 1000)}`
      }
    },
    className: String,
    init: Boolean
  },
  data () {
    return {
      oScroll: null
    }
  },
  watch: {
    init: {
      handler: function (newval) {
        newval && this.initScroll()
      },
      deep: true
    }
  },
  mounted () {
    this.init && this.initScroll()
  },
  destroyed () {
    if (this.oScroll !== null) {
      this.oScroll.destroy()
      this.oScroll = null
    }
  },
  methods: {
    ...mapMutations('SystemConfig', ['setMultipeSelectScollTop']),

    initScroll () {
      const self = this
      self.$nextTick(() => {
        if (self.oScroll !== null) {
          self.oScroll.destroy()
          self.oScroll = null
        }
        setTimeout(function () {
          const oPanelScroll = document.querySelector(`#${self.id}`)
          oPanelScroll && (self.oScroll = new Optiscroll(oPanelScroll))
          // 监听滚动获取滚高
          oPanelScroll.addEventListener('scroll', function (ev) {
            if (ev.detail && (ev.detail.scrollTop || ev.detail.scrollTop === 0)) {
              self.setMultipeSelectScollTop({
                attr: 'multipeSelectScollTop',
                val: ev.detail.scrollTop
              })
            }
          })
        }, 10)
      })
    }
  }
}
</script>

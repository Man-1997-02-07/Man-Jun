<template>
  <div
    v-show="isShow"
    :class="className">
    <div class="spinner">
      <span class="rect1" />
      <span class="rect2" />
      <span class="rect3" />
      <span class="rect4" />
      <span class="rect5" />
    </div>
  </div>
</template>
<script>
export default {
  props: {
    className: {
      type: String,
      default: 'vs-loading'
    },
    isShow: {
      type: Boolean,
      default: false
    }
  },
  mounted () {
  }
}
</script>
<style lang="scss" scoped>
// loading加载动画
	.vs-loading,
	.vs-inner-loading {
			z-index: 1999;
			background: rgba(255, 255, 255, .5);
			.spinner {
					margin: 0px auto;
					width: 38px;
					height: 40px;
					display: flex;
					justify-content: space-around;
					text-align: center;
					position: absolute;
					left: 50%;
					top: 50%;
					transform: translate3d(-50%, -50%, 0);
					span {
							display: inline-block;
							height: 100%;
							width: 3px;
							-webkit-animation: stretchdelay 1.2s infinite ease-in-out;
							animation: stretchdelay 1.2s infinite ease-in-out;
							background-color: #3583E3;
					}
					.rect2 {
							-webkit-animation-delay: -1.1s;
							animation-delay: -1.1s;
					}
					.rect3 {
							-webkit-animation-delay: -1.0s;
							animation-delay: -1.0s;
					}
					.rect4 {
							-webkit-animation-delay: -0.9s;
							animation-delay: -0.9s;
					}
					.rect5 {
							-webkit-animation-delay: -0.8s;
							animation-delay: -0.8s;
					}
			}
	}
	.vs-loading {
			position: fixed;
			left: 0;
			top:68px;
			right: 0;
			bottom: 0;
			z-index: 1000;
	}
	.vs-inner-loading {
			position: absolute;
			left: 0;
			top: 0;
			right: 0;
			bottom: 0;
	}
</style>

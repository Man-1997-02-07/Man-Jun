import VxBox from './src/main'

VxBox.install = function (Vue) {
  Vue.component(VxBox.name, VxBox)
}

export default VxBox

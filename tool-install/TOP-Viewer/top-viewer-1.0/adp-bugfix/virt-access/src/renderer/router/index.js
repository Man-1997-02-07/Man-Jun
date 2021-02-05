import Vue from 'vue'
import Router from 'vue-router'

const Home = resolve => require(['@/components/Home.vue'], resolve)

Vue.use(Router)

// 解决ElementUI导航栏中的vue-router在3.0版本以上重复点菜单报错问题
const originalPush = Router.prototype.push
Router.prototype.push = function push (location) {
  return originalPush.call(this, location).catch(err => err)
}
export default new Router({
  routes: [
    {
      path: '/',
      redirect: '/Login'
    },
    {
      path: '/Login',
      name: 'login-page',
      component: require('@/components/Login').default
    },
    {
      path: '/',
      component: Home,
      children: [
        {
          path: '/desktopVms',
          name: 'desktopVms',
          component: require('@/components/desktop/index.vue').default
        },
        {
          path: '/computer',
          name: 'computer',
          component: require('@/components/computer/index.vue').default
        },
        {
          path: '/vmScreenShot',
          name: 'vmScreenShot',
          component: require('@/components/computer/vmScreenShot').default
        },
        {
          path: '/netdisk',
          name: 'netdisk',
          component: require('@/components/netdisk/index').default
        }]
    }
  ]
})

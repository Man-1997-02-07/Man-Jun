module.exports = {
  env: {
    browser: true,
    es6: true,
    node: true
  },
  extends: [
    'plugin:vue/essential',
    'standard',
    'plugin:vue/recommended'
  ],
  globals: {
    Atomics: 'readonly',
    SharedArrayBuffer: 'readonly',
    
  },
  parserOptions: {
    ecmaVersion: 2018
  },
  plugins: [
    'vue'
  ],
  'rules': {
    // // allow paren-less arrow functions
    // 'arrow-parens': 0,
    // "space-before-function-paren": 0,
    // // allow async-await
    // 'generator-star-spacing': 0,
    // indent: 2,
    // // allow debugger during development
    // 'no-debugger': process.env.NODE_ENV === 'production' ? 2 : 0

    // 因项目中暂时存在许多不规范的代码,为防止导致项目报错,故去除部分代码规范的验证
    'eqeqeq': 'off',
    'vue/no-v-html': 'off',
    'no-debugger': 'off',
    'vue/html-closing-bracket-newline': 'off',
    'vue/no-unused-components': 'off',
    'vue/require-default-prop': 'off',
    'no-unused-vars': 'off',
    'no-unused-vars': 'off',
    "vue/attribute-hyphenation": 'off',
    'vue/require-valid-default-prop': 'off',
    'no-tabs': 'off',
    'camelcase': 'off',
    'no-undef': 'off',
    'no-useless-escape': 'off',
    'no-control-regex' :'off',
    'vue/attribute-hyphenation': 'off',
    'vue/order-in-components': 'off',
    'vue/prop-name-casing': 'off',
    'vue/attribute-hyphenation': 'off',
    'no-self-assign': 'off',
    'vue/require-prop-types': 'off', // prop必须定义类型
  }
}

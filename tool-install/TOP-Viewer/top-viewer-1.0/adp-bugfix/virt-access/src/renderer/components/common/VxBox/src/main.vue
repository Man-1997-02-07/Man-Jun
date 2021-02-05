<template>
  <el-row :class="['vx-box',className]">
    <el-row
      v-if="$slots.header || header"
      :class="['vx-box-header',{'line':headerLine}]">
      <slot name="header">
        {{ header }}
      </slot>
    </el-row>
    <el-row
      v-if="!empty"
      class="vx-box-body">
      <slot />
    </el-row>
    <el-row
      v-else
      class="vx-box-body empty-box">
      <p>暂无{{ header || '数据' }}</p>
    </el-row>
  </el-row>
</template>
<script>
export default {
  name: 'VxBox',
  props: {
    header: String,
    className: String,
    empty: {
      type: Boolean,
      default: false
    },
    headerLine: {
      type: Boolean,
      default: false
    }
  }
}
</script>
<style lang="scss" scoped>
  .vx-box {
    padding: 10px;

    .vx-box-body {
      height: calc(100% - 120px);
    }

    .vx-box-header {
      font-size: 16px !important;
    }
  }
  // 内容盒子
  .vx-box {
    border-radius: 10px;
    min-height: 232px;
    margin-bottom: 20px;
    cursor: pointer;
    // border:1px solid #ccc;
    height: 100%;
  }

  .vx-box-header {
    position: relative;
    margin: 0;
    font-weight: normal;
    font-size: 18px;

    .title {
      vertical-align: middle;
      color: #000;
    }

    .subtxt {
      height: 30px;
      font-size: 14px;
      vertical-align: middle;
      color: #df1130;

      i {
        font-size: 16px;
        vertical-align: middle;
      }
    }

    &.line {
      padding-bottom: 10px;
      // @include borderSign(bottom);
      border-bottom: 1px solid #e6e6e6;
    }

    .el-button {
      display: inline-block;
      vertical-align: middle;
      margin: 0 10px 0 0;
    }

    .search {
      display: inline-block;
      vertical-align: middle;
      width: 50%;
      // .el-input__inner{
      //     height: 30px;
      // }
      margin-left: -62px;
    }

    // .el-input__inner{
    //     height: 30px;
    // }
    .el-dropdown {
      vertical-align: middle;
    }

    .checkFont {
      padding: 5px 10px;

      .el-icon-search {
        font-size: 24px;
      }
    }

    h5 {
      display: inline-block;
    }
  }

  .vx-box-body {
    position: relative;
    padding-top: 10px;
    min-height: 190px;

    // display: flex;
    &:before,
    &:after {
      display: table;
      content: "";
    }

    &:after {
      clear: both;
    }
  }

  .vx-box-left {
    position: absolute;
    top: 50%;
    transform: translateY(-50%);
    // @include borderSign(right);

    .vx-box-lCon {
      height: 120px;
      text-align: center;

      span {
        font-size: 36px;
        line-height: 120px;
      }

      h3 {
        font-size: 36px;
        line-height: 90px;
        color: #df1130;
      }

      p {
        font-size: 20px;
        line-height: 0;
      }
    }
  }

  .vx-box-right {
    position: absolute;
    right: 0;
    top: 50%;
    transform: translateY(-50%);

    p {
      padding: 5px 5%;

      i.fr {
        margin-right: 20px;
      }
    }
  }
</style>

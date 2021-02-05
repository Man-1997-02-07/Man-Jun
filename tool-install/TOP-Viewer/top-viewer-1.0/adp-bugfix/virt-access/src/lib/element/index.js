import VueMessage from '@/components/message' // 自定义message提示
import VueMessageBox from '@/components/message-box' // 自定义confirm提示框
import {
  Row,
  Col,
  Card,
  Button,
  ButtonGroup,
  Radio,
  Dialog,
  RadioGroup,
  RadioButton,
  Checkbox,
  CheckboxGroup,
  Input,
  Select,
  Option,
  Tree,
  Switch,
  Form,
  FormItem,
  Table,
  TableColumn,
  Tag,
  Pagination,
  Message,
  MessageBox,
  Menu,
  Submenu,
  MenuItem,
  MenuItemGroup,
  Tabs,
  TabPane,
  Breadcrumb,
  BreadcrumbItem,
  DatePicker,
  Dropdown,
  DropdownMenu,
  DropdownItem,
  Tooltip,
  CheckboxButton,
  ColorPicker,
  Popover,
  OptionGroup,
  Progress,
  Cascader,
  Collapse,
  CollapseItem,
  InputNumber,
  Upload,
  TimePicker,
  TimeSelect,
  Carousel,
  CarouselItem,
  Alert,
  Divider,
  Loading,
  Badge,
  Link,
  Steps,
  Step,
  Slider,
  Transfer
} from 'element-ui'

const element = {
  install: function (Vue) {
    Vue.prototype.$alert = MessageBox.alert
    Vue.prototype.$prompt = MessageBox.prompt
    Vue.prototype.$elMessage = Message
    Vue.prototype.$message = VueMessage
    Vue.prototype.$confirm = VueMessageBox.confirm
    Vue.use(Row)
    Vue.use(Col)
    Vue.use(Card)
    Vue.use(Divider)
    Vue.use(Button)
    Vue.use(ButtonGroup)
    Vue.use(Radio)
    Vue.use(RadioGroup)
    Vue.use(RadioButton)
    Vue.use(Checkbox)
    Vue.use(CheckboxGroup)
    Vue.use(Input)
    Vue.use(Select)
    Vue.use(Option)
    Vue.use(Slider)
    Vue.use(Switch)
    Vue.use(DatePicker)
    Vue.use(Form)
    Vue.use(FormItem)
    Vue.use(Table)
    Vue.use(TableColumn)
    Vue.use(Tag)
    Vue.use(Pagination)
    Vue.use(Menu)
    Vue.use(Submenu)
    Vue.use(MenuItem)
    Vue.use(MenuItemGroup)
    Vue.use(Tabs)
    Vue.use(TabPane)
    Vue.use(Breadcrumb)
    Vue.use(BreadcrumbItem)
    Vue.use(Dropdown)
    Vue.use(DropdownMenu)
    Vue.use(DropdownItem)
    Vue.use(Dialog)
    Vue.use(Tooltip)
    Vue.use(CheckboxButton)
    Vue.use(ColorPicker)
    Vue.use(Popover)
    Vue.use(OptionGroup)
    Vue.use(Progress)
    Vue.use(Cascader)
    Vue.use(Tree)
    Vue.use(Collapse)
    Vue.use(CollapseItem)
    Vue.use(Upload)
    Vue.use(InputNumber)
    Vue.use(TimePicker)
    Vue.use(TimeSelect)
    Vue.use(Carousel)
    Vue.use(CarouselItem)
    Vue.use(Alert)
    Vue.use(Loading.directive)
    Vue.use(Badge)
    Vue.use(Link)
    Vue.use(Steps)
    Vue.use(Step)
    Vue.use(Transfer)
  }
}
export default element

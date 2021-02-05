let VSStatus = {
  UK: '未知',
  cluster: {
    UK: '未知',
    0: '未知',
    1: '健康',
    2: '告警',
    3: '不可用'
  },
  volume: {
    UK: '未知',
    0: '不可用',
    1: '健康',
    2: '降级',
    3: '失效',
    4: '挂起',
    5: '已删除'
  },
  replicas: {
    UK: '未知',
    0: '未知',
    1: '健康',
    2: '降级'
  },
  component: {
    UK: '未知',
    0: '未知',
    1: '已连接',
    2: '失去连接',
    3: '过期',
    11: '迁移中'
  },
  snapshot: {
    0: '正常',
    1: '删除中',
    2: '无效'
  },
  interface: {
    UK: '未知',
    up: '在线',
    down: '离线'
  },
  host: {
    UK: '未知',
    0: '未知',
    1: '健康',
    2: '故障',
    3: '故障',
    4: '宕机'
  },
  controllerHost: {
    UK: '未知',
    0: '未知',
    1: '健康',
    2: '不健康',
    3: '掉线',
    4: '宕机'
  },
  strategyHost: {
    UK: '未知',
    0: '未知',
    1: '健康',
    2: '不健康',
    3: '掉线',
    4: '宕机'
  },
  stroageHost: {
    UK: '未知',
    0: '未知',
    1: '健康',
    2: '不健康',
    3: '掉线',
    4: '宕机'
  },
  physicalHost: {
    UK: '未知',
    busy: '使用中',
    ready: '就绪',
    invalid: '故障',
    55: '不可迁移'
  },
  vmTask: {
    UK: '未知',
    SUCCESS: '成功',
    FAILURE: '失败',
    RUNNING: '运行中'
  },
  compute: {
    // 虚拟机状态-- 0: nostate, 1: running, 2: blocked, 3: paused, 4: shutdown, 5: shutoff, 6: crashed, 7: pmsuspend
    UK: '未知',
    running: '运行',
    shutdown: '关机',
    nostate: '无状态',
    blocked: '阻塞',
    crashed: '崩溃',
    shutoff: '关机',
    pmsuspend: '睡眠',
    paused: '挂起'
  },
  agent: {
    // 虚拟机agent-- "disconnected", "connected", "pending"
    disconnected: '未连接',
    connected: '已连接',
    pending: '未决'
  },
  hostMode: {
    UK: '未知',
    0: '正常',
    20: '维护',
    21: '退役'
  },
  USB: {
    UK: '未知',
    0: '正常',
    1: '主机错误',
    2: 'USB被拔出'
  },
  disk: {
    0: '未知',
    1: '正常',
    3: '停止中',
    4: '停止',
    5: '错误',
    6: '删除'
  },
  imageState: {
    UK: '未知',
    0: '正常',
    2: '导入中',
    3: '导入失败'
  },
  container: {
    running: '正常',
    pending: '等待',
    other: '异常'
  },
  balance: {
    UK: '未知',
    true: '启用',
    false: '停用'
  },
  router: {
    UK: '未知',
    true: '启用',
    false: '禁用'
  }
}
let VSFlag = {
  UK: '未知',
  volume: {
    UK: '未知',
    0: '正常',
    1: '重建失败',
    3: '重建中',
    7: '重建成功',
    9: '克隆失败',
    10: '克隆中',
    11: '克隆完成',
    12: '克隆成功',
    13: '备份中',
    14: '备份成功',
    15: '备份失败',
    16: '备份恢复中',
    17: '备份恢复成功',
    18: '备份恢复失败'
  },
  vm: {
    // 虚拟机动作--0: noaction, 1: cloning, 2: save, 3: migrating, 4: pending, 5: importing 6.unknown 7.restoring
    unknown: '未知',
    noaction: '无动作',
    cloning: '克隆中',
    migrating: '迁移中',
    importing: '导入中',
    save: '休眠',
    pending: '待定',
    restoring: '备份恢复中',
    // 新加 -attr
    importfail: '导入失败',
    exporting: '导出中',
    exportfail: '导出失败'

  }, // 本地化
  data_localization: {
    stopped: '无动作',
    started: '',
    completed: ''
  },
  HA: {
    0: 'auto',
    1: 'on',
    2: 'off'
  }
}
var VSNames = {
  UK: '未知',
  DevType: {
    block: 'SCSI块设备',
    target: 'iSCSI Target',
    share: '共享设备'
  },
  DevShares: {
    UK: '未知',
    nfs: 'NFS共享',
    rest: 'Rest共享',
    smb: 'SMB共享',
    s3: 's3共享',
    swift: 'Swift共享'
  },
  DataType: {
    UK: '未知',
    replica: '副本',
    ec: '纠删码'
  },
  DriveType: {
    UK: '未知',
    HDD: '机械硬盘',
    SSD: '固态硬盘'
  },
  Safety: {
    UK: '未知',
    first: '弱一致',
    second: '最终一致',
    third: '强一致'
  },
  ScrubLevels: {
    UK: '未知',
    fast: '快速',
    light: '轻量',
    full: '完全'
  },
  ClusterType: {
    UK: '未知',
    strategy: '策略主机',
    storage: '存储主机',
    controller: '应用主机'
  },
  Alarm: {
    UK: '未知',
    1: '提示',
    2: '次要',
    3: '重要',
    4: '紧急'
  },
  task: {
    UK: '未知',
    1: '快照',
    2: '备份',
    3: '元数据'
  },
  taskObj: {
    UK: '未知',
    1: '存储池',
    2: '存储卷',
    3: '元数据'
  },
  DiskType: {
    DiskDevs: '缓存层',
    DataDevs: '热数据层',
    CacheDevs: '数据层'
  },
  resourceType: {
    DiskDevs: '可用硬盘：',
    DataDevs: '缓存设备：',
    CacheDevs: '数据设备：'
  },
  alarmModel: {
    1: '设备模块',
    2: '系统模块',
    3: '服务模块'
  },
  alarmSubModel: {
    4: 'CPU',
    5: '负载',
    6: '内存',
    7: '网络',
    8: '磁盘',
    9: '主机',
    10: '存储',
    11: '应用',
    12: '策略',
    13: '实例',
    14: '虚拟机'
  },
  cloneMethod: {
    0: '链接克隆',
    1: '完全克隆（保留快照）',
    2: '完全克隆（合并快照）'
  },
  attrName: {
    DataType: '数据类型',
    DevType: '设备类型',
    DriveType: '硬盘类型',
    Encrypto: '加密',
    ReadBytesLimit: '读限制',
    ReadIOPSLimit: '读IOPS限制',
    Safety: '安全级别',
    ScheduleOption: '调度',
    ThinProvision: '精简配置',
    VmCache: '缓存',
    WriteBytesLimit: '写限制',
    WriteIOPSLimit: '写IOPS限制',
    replica: '副本数',
    SecretKey: '密钥',
    TargetACL: 'TargetACL',
    filter_label: '标签过滤',
    ShareType: '共享卷类型',
    Zone: 'Zone',
    IOPS: '设置QOS'
  },
  sidebar: {
    virtual_network: '网络虚拟化',
    cloud_desk: '云桌面',
    vsphere: 'vsphere'
  },
  vmStatus: {
    UK: '未知',
    running: '运行',
    save: '休眠',
    suspend: '暂停',
    stop: '停止',
    other: '其他'
  }
}
var VSError = {
  SmartDisk: {
    UK: '未知',
    1: ['底层数据读取错误率'],
    2: ['磁盘读写通量性能'],
    3: ['主轴起旋时间'],
    4: ['启停计数'],
    5: ['重映射扇区计数', '退役块计数'],
    6: ['读取通道余量'],
    7: ['寻道错误率'],
    8: ['寻道性能'],
    9: ['通电时间累计'],
    10: ['主轴起旋重试次数'],
    11: ['磁头校准重试计数'],
    12: ['通电周期计数'],
    13: ['软件读取错误率'],
    170: ['坏块增长计数'],
    171: ['编程失败块计数'],
    172: ['擦写失败块计数'],
    173: ['磨损平衡操作次数（平均擦写次数）'],
    174: ['意外失电计数'],
    177: ['磨损范围对比值'],
    180: ['未用的备用块计数'],
    181: ['编程失败计数 ', '非4KB对齐访问数'],
    182: ['擦写失败计数'],
    183: ['串口降速错误计数'],
    184: ['I/O错误检测与校正', '点到点错误检测计数', '原始坏块数'],
    185: ['磁头稳定性'],
    186: ['感应运算振动检测'],
    187: ['无法校正的错误'],
    188: ['命令超时'],
    189: ['高飞写入', '出厂坏块计数'],
    190: ['气流温度'],
    191: ['冲击错误率'],
    192: ['断电返回计数'],
    193: ['磁头加载/卸载计数'],
    194: ['温度'],
    195: ['硬件ECC校正', '实时无法校正错误计数', '编程错误块计数'],
    196: ['重映射事件计数', '擦除错误块计数'],
    197: ['当前待映射扇区计数', '读取错误块计数（不可修复错误）'],
    198: ['脱机无法校正的扇区计数', '总读取页数'],
    199: ['Ultra ATA访问校验错误率', '总写入页数'],
    200: ['写入错误率', '多区域错误率', '总读取指令数'],
    201: ['脱道错误率', '逻辑读取错误率', '写入指令总数'],
    202: ['数据地址标记错误', '剩余寿命', '闪存总错误bit数'],
    203: ['软件ECC错误数', '校正bit错误的总读取页数'],
    204: ['软件ECC校正', '坏块满标志'],
    205: ['热骚动错误率', '最大可编程/擦除次数'],
    206: ['磁头飞行高度', '底层数据写入出错率', '最小擦写次数'],
    207: ['主轴过电流', '最大擦写次数'],
    208: ['主轴电机重启次数', '平均擦写次数'],
    209: ['脱机寻道性能', '剩余寿命百分比'],
    210: ['斜坡加载值', '坏块管理错误日志'],
    211: ['写入时振动', 'SATA主机接口CRC写入错误计数'],
    212: ['写入时冲击', 'SATA主机接口读取错误计数'],
    220: ['盘片偏移量'],
    221: ['冲击错误率'],
    222: ['磁头寻道时间累计'],
    223: ['磁头加载/卸载重试计数'],
    224: ['磁头阻力'],
    225: ['主机写入数据量'],
    226: ['磁头加载时间累计'],
    227: ['扭矩放大计数'],
    228: ['断电返回计数'],
    230: ['GMR磁头振幅'],
    231: ['温度', '剩余寿命'],
    232: ['寿命余量', '预留空间剩余量'],
    233: ['通电时间累计', '介质磨耗指数'],
    240: ['磁头飞行时间', '传输错误率'],
    241: ['LBA写入总数', '写入剩余寿命'],
    242: ['LBA读取总数', '读取剩余寿命'],
    250: ['读取错误重试率'],
    254: ['自由坠落保护']
  }
}
let VXCLASSNAME = {
  UK: 'default',
  clusterStatus: {
    0: 'default',
    1: 'health',
    2: 'wraning',
    3: 'danger'
  }
}
let VXSTATUS = {
  UK: '未知',
  cluster: {
    0: '未知',
    1: '健康',
    2: '告警',
    3: '不可用'
  }
}
let VXICONS = [
  {
    keyWord: 'Linux',
    class: 'os_linux'
  },
  {
    keyWord: 'Windows',
    class: 'os_windows'
  },
  {
    keyWord: 'Ubuntu',
    class: 'os_ubuntu'
  },
  {
    keyWord: 'CentOS',
    class: 'os_centOS'
  },
  {
    keyWord: 'Fedora',
    class: 'os_fedora'
  },
  {
    keyWord: 'Red Hat',
    class: 'os_redhat'
  },
  {
    keyWord: 'Debian',
    class: 'os_debian'
  },
  {
    keyWord: 'SUSE',
    class: 'os_opensuse'
  },
  {
    keyWord: 'FreeBSD',
    class: 'os_freebsd'
  },
  {
    keyWord: 'NetBSD',
    class: 'os_netbsd'
  },
  {
    keyWord: 'OpenBSD',
    class: 'os_openbsd'
  },
  {
    keyWord: 'Solaris',
    class: 'solaris'
  },
  {
    keyWord: 'Mac OS X',
    class: 'os_apple'
  }
]

export {
  VSStatus, VSFlag, VSNames, VSError, VXCLASSNAME, VXSTATUS, VXICONS
}

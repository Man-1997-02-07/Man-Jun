// 共享卷列表数据处理
export function handleShareVolumeListData (data) {
  // const { params, data } = options
  const params = {}
  if (data == '' || !data || !data.list) { return [] }
  const vmName = {
    GDT: '云桌面：',
    GUS: '云服务器：',
    GCD: '桌面虚拟机：',
    GNW: '网络服务器：',
    GNGTP: 'NGTP服务器：',
    GVPROXY: '负载均衡器：'
  }
  const vmTempName = {
    GDT: '桌面池模板：',
    GUS: '云服务器模板：',
    default: '模板：'
  }
  data.list.forEach((item, idx) => {
    item.volume.Attr = item.volume.Attr || {}
    item.index = idx + 1 + (params.page_num || 0) * (params.page_size || 0)
    item.name = item.volume.Name
    item.uuid = item.volume.UUID
    item.clonename = item.name.replace(params.filter_name, '<i class="filters-string">' + params.filter_name + '</i>')
    item.capacity = item.volume.Capacity / (1024 * 1024 * 1024) + 'G'
    item.usedCapacity = (item.volume.Used / (1024 * 1024 * 1024)) > 1 ? (item.volume.Used / (1024 * 1024 * 1024)).toFixed(2) + 'G' : (item.volume.Used / (1024 * 1024)).toFixed(2) + 'M'
    item.username = ''
    item.pool_name = item.volume.PoolName
    item.pool_uuid = item.volume.PoolUUID
    item.volume_type = item.volume.Type === 'share' ? 'rest' : item.volume.Type
    // 对本存储池有使用权限的租户命名空间（若为空则为全局共享）
    item.ownerPoolTenantNamespaces = item.volume.ShareNsUUIDs
    item.ischeck = false
    // item.flagTxt = VSFlag.shareVolume[item.volume.EventState] ? VSFlag.shareVolume[item.volume.EventState] : VSFlag.shareVolume['UK']
    item.flagClass = [0, 4, 7, 12, 15].includes(item.volume.EventState) ? 'success' : [2, 6, 9, 11, 14].includes(item.volume.EventState) ? 'warning' : [1, 5, 8, 13, 16].includes(item.volume.EventState) ? 'danger' : 'default'
    item.accessPath = item.volume.AccessPath
    item.attr = item.volume.Attr
    item.Attr = item.volume.Attr
    item.deviceResource = item.volume.Attr.VirtualMachineDevice && item.volume.Attr.VirtualMachineType ? vmName[item.volume.Attr.VirtualMachineType] + item.volume.Attr.VirtualMachineName + '-/dev/' + item.volume.Attr.VirtualMachineDevice : item.volume.Attr.ContainerName ? '容器：' + item.volume.Attr.ContainerName : Boolean(item.volume.Attr.VirtualMachineName) && Boolean(item.volume.Attr.VirtualMachineTemplate) ? vmTempName[item.volume.Attr.VirtualMachineType || 'default'] + item.volume.Attr.VirtualMachineName : '-'
    // 1.不被虚拟机等设备使用 2.不被作为租户的屏幕截图和内存快照空间 3.不是克隆卷的的母卷
    item.isCanDel = !item.volume.Children && (item.deviceResource === '-') && !item.volume.Attr.ScreenshotVolume && !item.volume.Attr.DomainMemoryVolume
    // 被虚拟机使用的或者非block类型的卷不能操作快照 2.普通用户不能操作共享卷快照
    item.disabledOperateSnapshot = item.deviceResource !== '-' || item.volume.Type !== 'block' || params.isuser
    item.cTime = new Date(item.volume.Mtime * 1000).Format('yyyy-MM-dd-hh:mm:ss')
    item.zone_name = item.volume.ZoneName
  })
  return data.list
}

'use strict'

import { app, globalShortcut, BrowserWindow, ipcMain } from 'electron'

var os = require('os')
/**
 * Set `__static` path to static files in production
 * https://simulatedgreg.gitbooks.io/electron-vue/content/en/using-static-assets.html
 */
if (process.env.NODE_ENV !== 'development') {
  global.__static = require('path').join(__dirname, '/static').replace(/\\/g, '\\\\')
}

const electron = require('electron')
const {shell} = require('electron')
let mainWindow
const winURL = process.env.NODE_ENV === 'development'
  ? `http://localhost:9080`
  : `file://${__dirname}/index.html`

function createWindow () {
  /**
   * Initial window options
   * 创建窗口
   */
  electron.Menu.setApplicationMenu(null) // 去除菜单
  mainWindow = new BrowserWindow({
    icon: './static/icon.ico',
    height: 860,
    minHeight: 520, // 最小高度
    minWidth: 850, // 最小高度
    width: 1270,
    backgroundColor: '#27274e',
    useContentSize: true,
    maximizable: true,
    minimizable: true,
    resizable: true,
    autoHideMenuBar: true,
    show: false,
    // frame: false,
    // fullscreen: true,
    fullscreen: !os.platform().includes('win'), // 打开时就全屏
    DevTools: true,
    webPreferences: {
      DevTools: true,
      nodeIntegration: true, // 在网页中集成Node
      webSecurity: false}
  })
  // mainWindow.webContents.openDevTools()
  mainWindow.loadURL(winURL)
  mainWindow.once('ready-to-show', () => {
    mainWindow.show()
  })
  // 关闭时发送给渲染进程
  let closeFlag = false
  mainWindow.on('close', (event) => {
    mainWindow.webContents.send('close-screen')
    if (!closeFlag) {
      event.preventDefault()
      // event.sender.send('reply', 'Ok')
    }
  })

  ipcMain.on('cleanok', (event, arg) => {
    closeFlag = true
    app.quit()
  })

  ipcMain.on('fullScreen', () => {
    mainWindow.setFullScreen(true)
  })
  ipcMain.on('closed', () => {
    mainWindow.close()
  })
  ipcMain.on('maximize', (event, args) => {
    mainWindow.setFullScreen(false)
    mainWindow.maximize()
    event.sender.send('reply', 'Ok')
  })

  ipcMain.on('normal', (event) => {
    mainWindow.setFullScreen(false)
    mainWindow.restore()
    mainWindow.setSize(1270, 890)
    mainWindow.center()
  })
  ipcMain.on('openConsole', () => {
    let focusWin = BrowserWindow.getFocusedWindow()
    focusWin && focusWin.toggleDevTools()
  })
}

// const ex = process.execPath  // 此方法linux无法使用
// // const exeName = path.basename(process.execPath)
// app.setLoginItemSettings({
//   openAtLogin: true,
//   path: ex,
//   args: [
//     // '--processStart', `"${exeName}"`
//   ]
// })

// var AutoLaunch = require('auto-launch')
// var minecraftAutoLauncher = new AutoLaunch({
//   name: 'virt-access'
//   // path: '/Applications/Minecraft.app',
// })

// minecraftAutoLauncher.enable()
if (os.release().includes(7)) {
  // win7下 ，关闭硬件加速
  app.disableHardwareAcceleration()
}
app.commandLine.appendSwitch('disable-features', 'OutOfBlinkCors')
app.commandLine.appendSwitch('--ignore-certificate-errors', 'true')
app.commandLine.appendSwitch('ignore-certificate-errors')
app.on('ready', () => {
  createWindow()
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', (e) => {
  if (mainWindow === null) {
    createWindow()
  }
})
/**
 * Auto Updater
 *
 * Uncomment the following code below and install `electron-updater` to
 * support auto updating. Code Signing with a valid certificate is required.
 * https://simulatedgreg.gitbooks.io/electron-vue/content/en/using-electron-builder.html#auto-updating
 */

/*
import { autoUpdater } from 'electron-updater'

autoUpdater.on('update-downloaded', () => {
  autoUpdater.quitAndInstall()
})

app.on('ready', () => {
  if (process.env.NODE_ENV === 'production') autoUpdater.checkForUpdates()
})
 */

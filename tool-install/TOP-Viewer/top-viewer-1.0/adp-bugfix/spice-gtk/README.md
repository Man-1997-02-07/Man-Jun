#### 一、搭建编译环境
##### 1.1 windows安装msys2
去清华大学下载并根据教程：https://mirrors.tuna.tsinghua.edu.cn/help/msys2/

##### 1.2 msys2配置国内源
编辑`/etc/pacman.d/mirrorlist.mingw32`，修改为：
```
##
## 32-bit Mingw-w64 repository mirrorlist
##

## Primary
## msys2.org
Server = http://mirrors.ustc.edu.cn/msys2/mingw/i686/
Server = http://repo.msys2.org/mingw/i686
Server = http://downloads.sourceforge.net/project/msys2/REPOS/MINGW/i686
Server = http://www2.futureware.at/~nickoe/msys2-mirror/i686/
```
编辑`/etc/pacman.d/mirrorlist.mingw64`，修改为：
```
##
## 64-bit Mingw-w64 repository mirrorlist
##

## Primary
## msys2.org
Server = http://mirrors.ustc.edu.cn/msys2/mingw/x86_64/
Server = http://repo.msys2.org/mingw/x86_64
Server = http://downloads.sourceforge.net/project/msys2/REPOS/MINGW/x86_64
Server = http://www2.futureware.at/~nickoe/msys2-mirror/x86_64/
Server = http://mirror.bit.edu.cn/msys2/REPOS/
```
编辑`/etc/pacman.d/mirrorlist.msys`，在文件开头添加：
```
##
## MSYS2 repository mirrorlist
##

## Primary
## msys2.org
Server = http://mirrors.ustc.edu.cn/msys2/msys/$arch/
Server = http://repo.msys2.org/msys/$arch
Server = http://downloads.sourceforge.net/project/msys2/REPOS/MSYS2/$arch
Server = http://www2.futureware.at/~nickoe/msys2-mirror/msys/$arch/
```
然后执行 pacman -Sy 刷新软件包数据即可。

#### 二、安装依赖包
编译64bit，通过MSYS2 MinGW 64-bit启动msys2的shell; 编译32bit，通过MSYS2 MinGW 32-bit启动msys2的shell;

##### 安装
```
pacman -S make git automake autoconf
```

##### 安装64bit的依赖包:
```
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-cmake mingw-w64-x86_64-cairo mingw-w64-x86_64-pixman mingw-w64-x86_64-openssl mingw-w64-x86_64-glib2 mingw-w64-x86_64-nspr mingw-w64-x86_64-nss mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-json-glib mingw-w64-x86_64-gstreamer mingw-w64-x86_64-gst-plugins-bad mingw-w64-x86_64-gst-plugins-base mingw-w64-x86_64-gst-plugins-good mingw-w64-x86_64-gst-plugins-ugly mingw-w64-x86_64-gtk3 libltdl mingw-w64-x86_64-libusb mingw-w64-x86_64-usbredir mingw-w64-x86_64-nss mingw-w64-x86_64-cyrus-sasl mingw-w64-x86_64-lz4 mingw-w64-x86_64-phodav mingw-w64-x86_64-dbus-glib mingw-w64-x86_64-gobject-introspection mingw-w64-x86_64-phodav mingw-w64-x86_64-vala mingw-w64-x86_64-meson mingw-w64-x86_64-ninja intltool mingw-w64-x86_64-icoutils dos2unix mingw-w64-x86_64-libgsf bison libutil-linux-devel mingw-w64-x86_64-gtk-doc 
```

##### 安装32bit的依赖包:
```
pacman -S mingw-w64-i686-gcc mingw-w64-i686-make mingw-w64-i686-cmake mingw-w64-i686-cairo mingw-w64-i686-pixman mingw-w64-i686-openssl mingw-w64-i686-glib2 mingw-w64-i686-nspr mingw-w64-i686-nss mingw-w64-i686-libjpeg-turbo mingw-w64-i686-json-glib mingw-w64-i686-gstreamer mingw-w64-i686-gst-plugins-bad mingw-w64-i686-gst-plugins-base mingw-w64-i686-gst-plugins-good mingw-w64-i686-gst-plugins-ugly mingw-w64-i686-gtk3 libltdl mingw-w64-i686-libusb mingw-w64-i686-usbredir mingw-w64-i686-nss mingw-w64-i686-cyrus-sasl mingw-w64-i686-lz4 mingw-w64-i686-phodav mingw-w64-i686-dbus-glib mingw-w64-i686-gobject-introspection mingw-w64-i686-phodav mingw-w64-i686-vala mingw-w64-i686-meson mingw-w64-i686-ninja intltool mingw-w64-i686-icoutils dos2unix mingw-w64-i686-libgsf bison libutil-linux-devel mingw-w64-i686-gtk-doc
```

##### 设置PATH :
export PATH=$PATH:/usr/bin/core_perl/  

#### 三、编译
##### 编译spice-protocol
下载源码包并解压：
```
wget https://www.spice-space.org/download/releases/spice-protocol-0.14.1.tar.bz2
```

编译：
```
cd spice-protocol-0.14.1 

./configure
make 
make install
```

##### 编译spice-gtk
下载源码包并解压：
```
wget https://www.spice-space.org/download/gtk/spice-gtk-0.38.tar.xz
```

编译：
```
cd spice-gtk-0.38 

meson builddir
cd builddir 
ninja -j4
ninja install
```
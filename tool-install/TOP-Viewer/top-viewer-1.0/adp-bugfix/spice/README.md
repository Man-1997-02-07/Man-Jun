## spice-server

> 与Qemu进行通信的相关spice服务

### Build Steps For Ubuntu 16.04

#### 1. 从源上安装相关依赖

```shell
apt install libjpeg8-dev libglib2.0-dev libgstreamer1.0-dev liblz4-dev  liborc-0.4-dev libgstreamermm-1.0-dev  libopus-dev libx264-dev libmad0-dev libbz2-dev libv4l-dev libvpx-dev libjack-jackd2-dev libsoup2.4-dev libpulse-dev gtk-doc-tools  yasm gstreamer1.0-tools libpixman-1-dev
```

####  2. 从第三方拉取相关依赖

```bash
## 下载相关源码
wget https://gstreamer.freedesktop.org/src/gst-libav/gst-libav-1.12.5.tar.xz
wget https://gstreamer.freedesktop.org/src/gst-plugins-good/gst-plugins-good-1.12.5.tar.xz
wget https://gstreamer.freedesktop.org/src/gst-plugins-ugly/gst-plugins-ugly-1.12.5.tar.xz

## 解压
tar -cvf gst-libav-1.12.5.tar.xz
tar -xvf gst-plugins-good-1.12.5.tar.xz
tar -xvf gst-plugins-ugly-1.12.5.tar.xz

## 编译gst-plugins-good
cd gst-plugins-good-1.12.5
./configure
make -j4
make install

## 编译gst-plugins-ugly
cd ../gst-plugins-ugly-1.12.5/
./configure
make -j4 
make install

## 编译gst-libav
cd ../gst-libav-1.12.5/
./configure
make -j4 
make install

## 将动态库文件拷贝到指定目录
cp /usr/local/lib/gstreamer-1.0/*   /usr/lib/x86_64-linux-gnu/gstreamer-1.0/
```

#### 3.编译spice-protocol

```shell
cd adp/spice-protocol
chmod +x configure
./configure
make
make install
```

#### 4.安装pyparsing

```shell
apt install python-pyparsing
```

#### 5.编译spice

```shell
cd ../spice
chmod +x configure
./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --libdir=/usr/lib  --enable-lz4 --enable-gstreamer=1.0  --enable-static
make -j4
make install
```

#### QA：

1. 如果编译报错

   * 下载spice官方代码

     > wget https://www.spice-space.org/download/releases/spice-0.14.3.tar.bz2

   * 用官方代码中的spice-common替代项目代码中的spice-common
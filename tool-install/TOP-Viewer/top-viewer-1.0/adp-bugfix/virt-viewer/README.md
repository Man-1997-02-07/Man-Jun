### Build Spice Connector

#### 1.Prepare
```
OS: Fedora 29

Packages:
    yum groupinstall "Development Tools"
    yum install mingw* wine intltool glib2-devel icoutils msitools
```

#### 2.Compile
```
cd virt-viewer

32bit: 
  mingw32-configure --with-gtk=3.0 --with-spice-gtk
  mingw32-make
  mingw32-make install

64bit:
  mingw64-configure --with-gtk=3.0 --with-spice-gtk
  mingw64-make
  mingw64-make install
```

#### 3.Build MSI
```
cd data

    make msi
```


call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
set MSYS=C:\MinGW\msys\1.0\bin
PATH=%MSYS%;%path%
set NASM=C:\Program Files\NASM\nasm.exe
PATH=%NASM%;%path%
set INCLUDE=%INCLUDE%
set LIB=%lib%  
bash -c "make OS=msvc ARCH=x86_64"
pause
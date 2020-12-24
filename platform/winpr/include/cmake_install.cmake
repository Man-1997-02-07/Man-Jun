# Install script for directory: C:/FreeRdp/freerdp-2.2.0/winpr/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/FreeRDP")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/winpr2/winpr" TYPE FILE FILES
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/asn1.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/bcrypt.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/bitstream.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/clipboard.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/cmdline.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/collections.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/comm.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/credentials.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/credui.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/crt.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/crypto.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/debug.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/dsparse.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/endian.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/environment.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/error.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/file.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/handle.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/heap.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/image.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/ini.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/input.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/interlocked.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/intrin.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/io.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/library.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/locale.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/memory.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/midl.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/ndr.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/nt.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/ntlm.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/pack.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/path.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/pipe.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/platform.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/pool.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/print.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/registry.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/rpc.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/sam.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/schannel.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/security.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/shell.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/smartcard.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/spec.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/ssl.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/sspi.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/sspicli.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/stream.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/string.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/strlst.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/synch.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/sysinfo.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/tchar.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/thread.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/timezone.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/user.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/windows.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/winhttp.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/winpr.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/winsock.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/wlog.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/wnd.h"
    "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/wtsapi.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/winpr2/winpr" TYPE FILE FILES "C:/FreeRdp/platform/winpr/include/winpr/version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/winpr2/winpr" TYPE FILE FILES "C:/FreeRdp/platform/winpr/include/winpr/wtypes.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/winpr2/winpr" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/winpr/include/winpr/tools" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()


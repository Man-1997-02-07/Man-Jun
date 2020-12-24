# Install script for directory: C:/FreeRdp/freerdp-2.2.0/include

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE FILE FILES
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/addin.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/altsec.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/api.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/assistance.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/autodetect.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/client.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/codecs.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/constants.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/display.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/dvc.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/error.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/event.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/extension.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/freerdp.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/graphics.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/heartbeat.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/input.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/license.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/listener.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/log.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/message.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/metrics.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/peer.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/pointer.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/primary.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/primitives.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/rail.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/scancode.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/secondary.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/session.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/settings.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/svc.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/types.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/update.h"
    "C:/FreeRdp/freerdp-2.2.0/include/freerdp/window.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE FILE FILES "C:/FreeRdp/platform/include/freerdp/version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE FILE FILES "C:/FreeRdp/platform/include/freerdp/build-config.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/cache" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/codec" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/crypto" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/gdi" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/locale" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/utils" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/client" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/server" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xheadersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/freerdp2/freerdp" TYPE DIRECTORY FILES "C:/FreeRdp/freerdp-2.2.0/include/freerdp/channels" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()


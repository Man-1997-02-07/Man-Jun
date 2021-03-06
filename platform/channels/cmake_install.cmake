# Install script for directory: C:/FreeRdp/freerdp-2.2.0/channels

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/FreeRdp/platform/channels/drdynvc/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/video/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/urbdrc/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/smartcard/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/remdesk/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/rdpsnd/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/rdpgfx/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/rdpei/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/rdpdr/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/rdp2tcp/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/rail/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/printer/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/geometry/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/encomsp/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/echo/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/drive/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/disp/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/cliprdr/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/audin/cmake_install.cmake")
  include("C:/FreeRdp/platform/channels/client/cmake_install.cmake")

endif()


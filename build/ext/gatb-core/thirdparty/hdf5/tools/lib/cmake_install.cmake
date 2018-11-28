# Install script for directory: /home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local/HDF_Group/HDF5/1.8.18")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "toolsheaders" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5trav.h;/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5tools.h;/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5tools_dump.h;/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5tools_utils.h;/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5tools_str.h;/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5tools_ref.h;/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5/h5diff.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/flegeai/workspace/Con10x/build/ext/gatb-core/include/hdf5" TYPE FILE FILES
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5trav.h"
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools.h"
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_dump.h"
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_utils.h"
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_str.h"
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5tools_ref.h"
    "/home/flegeai/workspace/Con10x/thirdparty/gatb-core/thirdparty/hdf5/tools/lib/h5diff.h"
    )
endif()


################################################################################
# COMMON
################################################################################

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS serialization)
find_package(HTSlib REQUIRED)

# We add the compilation options for the library
add_definitions (${gatb-core-flags})

# We add the path for extra libraries
link_directories (${gatb-core-extra-libraries-path})


set (SOURCE_DIR ${PROJECT_SOURCE_DIR}/common/src)
include_directories (${SOURCE_DIR} ${gatb-core-includes} ${Boost_INCLUDE_DIRS} ${HTSlib_INCLUDE_DIRS})
set(SRC_FILES ${SOURCE_DIR}/Fragment10x.cpp
              ${SOURCE_DIR}/IndexReads_Hashed.cpp
              ${SOURCE_DIR}/IndexReads_Interleaved.cpp)
ADD_LIBRARY(common ${SRC_FILES})

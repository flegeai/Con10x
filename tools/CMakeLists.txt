################################################################################
#  TOOLS
################################################################################

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET (dirlist "")
    FOREACH(child ${children})
      IF(IS_DIRECTORY ${curdir}/${child})
          LIST(APPEND dirlist ${child})
      ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

# We add the compilation options for the library
add_definitions (${gatb-core-flags})

# We add the gatb-core include directory
include_directories (${gatb-core-includes})

# We add the path for extra libraries
link_directories (${gatb-core-extra-libraries-path})

# common
include_directories (common)
link_directories (common)

# We get the list of subdirectories names
SUBDIRLIST(PROGRAMS ${PROJECT_SOURCE_DIR}/tools)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS serialization)

find_package(HTSlib REQUIRED)


# We create a target for each 
FOREACH (program ${PROGRAMS})
  set (PROGRAM_SOURCE_DIR ${PROJECT_SOURCE_DIR}/tools/${program}/src)
  include_directories (${PROGRAM_SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${HTSlib_INCLUDE_DIRS})
  file (GLOB_RECURSE  ProjectFiles  ${PROGRAM_SOURCE_DIR}/*.cpp)
  add_executable(${program} ${ProjectFiles})
  target_link_libraries(${program} common ${gatb-core-libraries} ${Boost_LIBRARIES} ${HTSlib_LIBRARIES})
ENDFOREACH (program)

################################################################################
#  INSTALLATION 
################################################################################
install (TARGETS ${PROGRAMS} DESTINATION bin)

#
# cmake library project by Pablo Speciale:
# https://github.com/pablospe/cmake-example-library
#


## Project environement variables

set(CMAKE_CXX_STANDARD 11) # Use C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Require (at least) it
set(CMAKE_CXX_EXTENSIONS OFF) # Don't use e.g. GNU extension (like -std=gnu++11) for portability

# Set PROJECT_NAME_UPPERCASE and PROJECT_NAME_LOWERCASE variables
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)

# Library name (by default is the project name)
if(NOT LIBRARY_NAME)
  set(LIBRARY_NAME ${PROJECT_NAME})
endif()

# Library folder name (by default is the project name in lowercase), Example: #include <foo/foo.h>
if(NOT LIBRARY_FOLDER)
  set(LIBRARY_FOLDER ${PROJECT_NAME_LOWERCASE})
endif()

# Set additional folder path
set(PROJECT_THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/thirdparty" CACHE PATH "Third party libraries")
set(PROJECT_UTILITIES_DIR "${PROJECT_SOURCE_DIR}/utilities" CACHE PATH "Utilities folder")

if(APPLE)
  set(OpenCL_SUBDIR "OpenCL/")    
else()
  set(OpenCL_SUBDIR "CL/")    
endif()

# Set Preamble file 
set(CLIC_PREAMBLE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_FOLDER}/preamble.cl" CACHE FILEPATH "CLIJ preamble file")
mark_as_advanced(CLIC_PREAMBLE_FILE)

# Set clij folder file 
set(CLIC_KERNELS_DIR "${PROJECT_THIRDPARTY_DIR}/clij/kernels" CACHE PATH "CLIJ kernels directory")
mark_as_advanced(CLIC_KERNELS_DIR)

## Configuration and Build options

# Set Code coverage options
option(BUILD_CODE_COVERAGE "Enable coverage reporting" OFF)
mark_as_advanced(BUILD_CODE_COVERAGE)
if(BUILD_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage")
endif()

# Set library type options (default: STATIC)
option(BUILD_SHARED_LIBS "Build ${LIBRARY_NAME} as a shared library." OFF)
message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")

# Set Test compilation (default: ON)
option(BUILD_TESTING "Build ${LIBRARY_NAME} Tests." ON)
message(STATUS "BUILD_TESTING: ${BUILD_TESTING}")

# Set Doc compilation (default: OFF) (NOT IMPLEMENTED)
option(BUILD_DOCUMENTATION "Build ${LIBRARY_NAME} Documentation." OFF)
message(STATUS "BUILD_DOCUMENTATION: ${BUILD_DOCUMENTATION} (NOT IMPLEMENTED)")

# Set OpenCL Standard version number (default: 120)
set(OPENCL_VERSION 120)
mark_as_advanced(OPENCL_VERSION)
message(STATUS "OPENCL_VERSION: ${OPENCL_VERSION}")

# Manage build type options (default: RELEASE)
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
  set(CMAKE_CONFIGURATION_TYPES "Release;Debug;MinSizeRel;RelWithDebInfo" CACHE STRING "" FORCE)
  message(STATUS "CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
  message(STATUS "CMAKE_GENERATOR: Multi-config")
else()
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  endif()
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "MinSizeRel" "RelWithDebInfo")
  message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  message(STATUS "CMAKE_GENERATOR: Single-config")
endif()
message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

# Configurations tag to avoid compilation colliding
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")




## Install and Uninstall configuration

# Create and configure 'clic.h'
#   - define OpenCL version
#   - include OpenCL headers
#   - define CLIC version
configure_file(
  "${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/${PROJECT_NAME_LOWERCASE}.h.in"
  "${PROJECT_SOURCE_DIR}/${LIBRARY_FOLDER}/core/includes/${PROJECT_NAME_LOWERCASE}.h"
  NO_SOURCE_PERMISSIONS @ONLY
)

# Introduce variables:
#   - CMAKE_INSTALL_LIBDIR
#   - CMAKE_INSTALL_BINDIR
#   - CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

# Include module with functions:
#   - write_basic_package_version_file(...)
#   - configure_package_config_file(...)
include(CMakePackageConfigHelpers)

# Layout. This works for all platforms:
#   - <prefix>/lib*/cmake/<PROJECT-NAME>
#   - <prefix>/lib*/
#   - <prefix>/include/
set(CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

# Configuration
set(GENERATED_DIR       "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(VERSION_CONFIG_FILE "${GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(PROJECT_CONFIG_FILE "${GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   - PROJECT_VERSION
write_basic_package_version_file( 
  "${VERSION_CONFIG_FILE}" VERSION "${${PROJECT_NAME}_VERSION}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   - TARGETS_EXPORT_NAME
#   - PROJECT_NAME
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in" "${PROJECT_CONFIG_FILE}"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
    PATH_VARS CMAKE_INSTALL_PREFIX CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR 
)

# Uninstall targets
configure_file("${PROJECT_SOURCE_DIR}/cmake/Uninstall.cmake.in" "${GENERATED_DIR}/Uninstall.cmake"
  IMMEDIATE @ONLY
)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${GENERATED_DIR}/Uninstall.cmake)



## Other?? 

# Always full RPATH (for shared libraries)
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling
if(BUILD_SHARED_LIBS)
  # use, i.e. don't skip the full RPATH for the build tree
  set(CMAKE_SKIP_BUILD_RPATH FALSE)

  # when building, don't use the install RPATH already
  # (but later on when installing)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

  # add the automatically determined parts of the RPATH
  # which point to directories outside the build tree to the install RPATH
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

  # the RPATH to be used when installing, but only if it's not a system directory
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
      set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  endif()
endif()


# CMake Registry
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeRegistry.cmake)

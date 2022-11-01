cmake_minimum_required(VERSION 3.5.1)

project(opendrive-cpp VERSION 1.0.0)
# MAJOR: 主版本号; 大量重写,这些重写使得无法实现向后兼容性
# MINOR: 次版本号; API接口修改,修正版或完全向后兼容的新版本
# PATCH: 补丁版本; 内部函数的修改而不影响API接口 

set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build opendrive-cpp shared library" ON)
option(BUILD_OPENDRIVECPP_TEST "Build opendrive-cpp unittest" OFF)

set(opendrive-cpp-type SHARED)
if (NOT BUILD_SHARED_LIBS)
  set(opendrive-cpp-type STATIC)
endif()

include(CMakePackageConfigHelpers)
include(cmake/package.cmake)
include(GNUInstallDirs)

include_directories(
  include
)

file(GLOB OPENDRIVE_CPP_SRCS
  "src/*.cc"
)

add_library(${TARGET_NAME} ${opendrive-cpp-type}
  ${OPENDRIVE_CPP_SRCS}
)

if(BUILD_OPENDRIVECPP_TEST)
  add_subdirectory(tests)
endif()

# #################################################################################
# config
# #################################################################################
configure_file("cmake/${TARGET_NAME}.pc.in" 
  "${PROJECT_BINARY_DIR}/pkgconfig/${TARGET_NAME}.pc" @ONLY)

configure_package_config_file(
  "cmake/${TARGET_NAME}-config.cmake.in"
  "${PROJECT_BINARY_DIR}/cmake/${TARGET_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${TARGET_NAME}"
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
  "cmake/${TARGET_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${PROJECT_BINARY_DIR}/pkgconfig/${TARGET_NAME}.pc"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/pkgconfig"
)

install(EXPORT ${TARGET_NAME}-targets
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${TARGET_NAME}"
)

install(FILES
  "${PROJECT_BINARY_DIR}/cmake/${TARGET_NAME}-config.cmake"
  "${PROJECT_BINARY_DIR}/cmake/${TARGET_NAME}-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/${TARGET_NAME}"
)

install(DIRECTORY include/hdmap/
  DESTINATION include/hdmap
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN "*.hpp"
)

install(TARGETS ${TARGET_NAME}
  EXPORT ${TARGET_NAME}-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)


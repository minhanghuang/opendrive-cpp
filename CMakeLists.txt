cmake_minimum_required(VERSION 3.5.1)

project(opendrive-cpp VERSION 1.1.0)

set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(Tinyxml2 REQUIRED tinyxml2)

include_directories(
  include
  ${Tinyxml2_INCLUDE_DIRS}
)

link_directories(
  ${Tinyxml2_LIBRARY_DIRS}
)

file(GLOB OPENDRIVE_CPP_SRCS
  "src/*.cc"
  "src/parser/*.cc"
  "src/common/*.cc"
  "src/common/spiral/*.cc"
)

add_library(${TARGET_NAME} ${opendrive-cpp-type}
  ${OPENDRIVE_CPP_SRCS}
)

target_link_libraries(${TARGET_NAME}
  ${Tinyxml2_LIBRARIES}
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

install(DIRECTORY include/opendrive-cpp/
  DESTINATION include/opendrive-cpp
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


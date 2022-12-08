cmake_minimum_required(VERSION 3.5.1)
project(opendrive-cpp-test VERSION 0.0.0)

set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Gtest REQUIRED gtest)
pkg_check_modules(Tinyxml2 REQUIRED tinyxml2)

include_directories(
  ${Gtest_INCLUDE_DIRS}
  ${Tinyxml2_INCLUDE_DIRS}
)

link_directories (
  ${Gtest_LIBRARY_DIRS}
  ${Tinyxml2_LIBRARY_DIRS}
)

SET(TEST_SOURCES
  common_test
  geometry_test
  parser_map_test
  parser_header_test
  parser_roadtype_test
  parser_roadlink_test
  parser_roadplanview_test
  parser_road_test
  parser_roadlanes_test
  parser_junction_test
  adapter_map_test
  adapter_road_test
)

FOREACH(test_src ${TEST_SOURCES})
  add_executable(${test_src} ${test_src}.cc)
  target_link_libraries(${test_src}
    ${Gtest_LIBRARIES}
    ${Tinyxml2_LIBRARIES}
    opendrive-cpp
    gtest
  )
ENDFOREACH(test_src)

# #################################################################################
# config
# #################################################################################
file(COPY
  "data/"
  DESTINATION "${PROJECT_BINARY_DIR}/data"
)

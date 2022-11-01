cmake_minimum_required(VERSION 3.5.1)
project(opendrive-cpp-test VERSION 0.0.0)

set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Gtest REQUIRED gtest)

include_directories(
  ${Gtest_INCLUDE_DIRS}
)

link_directories (
  ${Gtest_LIBRARY_DIRS}
)

SET(TEST_SOURCES
  opendrive_test 
)

FOREACH(test_src ${TEST_SOURCES})
  add_executable(${test_src} ${test_src}.cc)
  target_link_libraries(${test_src}
    ${Gtest_LIBRARIES}
    opendrive-cpp
    gtest
  )
ENDFOREACH(test_src)


#!/bin/bash

function main() {
  echo "opendrive-cpp build."
  cmake -B build -DBUILD_SHARED_LIBS=ON -DBUILD_OPENDRIVECPP_TEST=ON -DCMAKE_BUILD_TYPE=Debug .
  cmake --build build -j4
}

main "$@"


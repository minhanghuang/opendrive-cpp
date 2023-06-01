#!/bin/bash

function main() {
  echo "opendrive-cpp build."
  mkdir -p build && cd build
  cmake -DBUILD_SHARED_LIBS=ON -DBUILD_OPENDRIVECPP_TEST=ON -DCMAKE_BUILD_TYPE=Debug ..
  make -j4
}

main "$@"


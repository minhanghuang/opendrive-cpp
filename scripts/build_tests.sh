#!/bin/bash

current_path=$(cd $(dirname $0) && pwd)

function test() {
  mkdir -p build && cd build
  cmake -DBUILD_SHARED_LIBS=ON -DBUILD_OPENDRIVECPP_TEST=ON -DCMAKE_BUILD_TYPE=Debug ..
  make -j4
  local bin_path="$current_path/../build/tests/"
  for file in $bin_path/*
  do
    case "$file" in
      *test )
        $file
        ;;
      * );;
    esac
  done
}

function main () {
  test
  return
}

main "$@"


#!/bin/bash

current_path=$(cd $(dirname $0) && pwd)

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/opt/owner/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/owner/lib

function test() {
  cmake -B build -DBUILD_SHARED_LIBS=ON -DBUILD_OPENDRIVECPP_TEST=ON -DCMAKE_BUILD_TYPE=Debug .
  cmake --build build -j6
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


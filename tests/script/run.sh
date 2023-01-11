#!/bin/bash

current_path=$(cd $(dirname $0) && pwd)

export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/opt/owner/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/owner/lib

function test() {
  local bin_path="$current_path/.."
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


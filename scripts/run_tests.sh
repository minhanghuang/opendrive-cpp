#!/bin/bash

current_path=$(cd $(dirname $0) && pwd)

function test() {
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


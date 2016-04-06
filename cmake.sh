#!/bin/bash

mkdir -p bin
mkdir -p build

rm -R bin/*
rm -R build/*


cmake -H. -Bbuild -G"Kate - Unix Makefiles"


pushd build > /dev/null

if [ ! $# == 1 ]; then
    set $1 Release
fi

cmake -DCMAKE_BUILD_TYPE=$1 -DCMAKE_INSTALL_PREFIX=/usr/local ../
#make edit_cache


echo
echo "CMAKE_BUILD_TYPE     = $1"
echo "CMAKE_INSTALL_PREFIX = /usr/local"

popd >/dev/null

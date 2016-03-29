#!/bin/bash

mkdir -p bin
mkdir -p build

cmake --build build -- -j3 $1 $2 $3 $4 $5

# pushd build > /dev/null
#
# make -j3 $1
#
#
# popd > /dev/null

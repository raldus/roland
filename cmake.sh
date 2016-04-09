#!/bin/bash

mkdir -p bin
mkdir -p build

rm -Rf bin/*
rm -Rf build/*

if [ ! $# == 1 ]; then
    set $1 Release
fi

cmake -H. -Bbuild -G"Kate - Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 -DCMAKE_INSTALL_PREFIX=/usr/local

#!/bin/bash

mkdir -p bin
mkdir -p build

rm -Rf bin/*
rm -Rf build/*

if [ ! $# == 1 ]; then
    echo "Usage: $0 Release (or Debug)"
    exit 1
fi

cmake -H. -Bbuild -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 -DCMAKE_INSTALL_PREFIX=/usr/local

cmake --build build --target help

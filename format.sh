#!/bin/bash

find src -name "*.cpp" -print -exec clang-format -i {} \; -or -name "*.h" -print -exec clang-format -i {} \;

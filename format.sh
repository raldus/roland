#!/bin/bash

find src -name "*.cpp" -print -exec clang-format -i {} \; -or -name "*.hpp" -print -exec clang-format -i {} \;

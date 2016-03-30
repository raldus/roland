#!/bin/bash

find src -name '*.h'   -exec bash -c 'expand -t 4 "$0" > /tmp/expand~ && mv /tmp/expand~ "$0"' {} \;
find src -name '*.cpp' -exec bash -c 'expand -t 4 "$0" > /tmp/expand~ && mv /tmp/expand~ "$0"' {} \;

rm -f /tmp/expand~

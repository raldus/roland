#!/bin/bash

echo "# Changelog" > CHANGELOG.md

git show --tags --quiet \
--pretty=format:'%ad %n[view tag](https://github.com/raldus/roland/commit/%H) %n%n***%n' \
--reverse >> CHANGELOG.md


#git log\
#    --pretty=format:'* [view commit](http://github.com/raldus/libxstd/commit/%H)' \
#    --reverse
#| grep "#changelog" >> CHANGELOG.md

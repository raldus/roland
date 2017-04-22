#!/bin/bash
# Author:Andrey Nikishaev

echo "# CHANGELOG"

git tag -l | sort -V -u -r | while read TAG ; do
    echo
    if [ $NEXT ];then
        echo "## $NEXT"
    else
        echo "### current"
    fi
    GIT_PAGER=cat git log --no-merges --format=" * %s" $TAG..$NEXT
    NEXT=$TAG
done
FIRST=$(git tag -l | head -1)
echo
echo "## $FIRST"
GIT_PAGER=cat git log --no-merges --format=" * %s" $FIRST 

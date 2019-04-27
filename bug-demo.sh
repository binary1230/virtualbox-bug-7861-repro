#!/bin/bash

set -e -x

rm -rf a0 a1 a2
mkdir -p a0/subdir a1/subdir
echo HI >a0/subdir/file
echo HI >a1/subdir/file
mv a1 a2
mv a0 a1
rm a1/subdir/file
cat a2/subdir/file

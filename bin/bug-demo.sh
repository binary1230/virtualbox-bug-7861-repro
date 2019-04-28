#!/bin/bash

# Repro code for https://www.virtualbox.org/ticket/8761
# This is a version in bash which is simpler but might call more syscalls
# see the C repro program, bug-demo.c for a version that minimizes syscalls

set -e -x

rm -rf a0 a1 a2
mkdir -p a0/subdir a1/subdir
echo HI >a0/subdir/file
echo HI >a1/subdir/file
mv a1 a2
mv a0 a1
rm a1/subdir/file
cat a2/subdir/file

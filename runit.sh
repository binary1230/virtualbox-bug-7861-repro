#!/bin/bash

set -e -x

rm -rf a0 a1 a2

gcc bug.c/bugx.c -o bugx
./bugx

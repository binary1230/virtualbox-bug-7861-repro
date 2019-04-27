#!/bin/bash

set -e -x

rm -rf a0 a1 a2

gcc bug-demo.c -o bug-demo
./bug-demo

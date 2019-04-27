#!/bin/bash

set -e -x

gcc bug-demo.c -o bug-demo

# cleanup
rm -rf a0 a1 a2
./bug-demo

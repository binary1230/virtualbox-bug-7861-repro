#!/bin/bash


FS_TYPE=`mount | grep $(df  --output=source . | tail -1) | cut -f 5 -d ' '`
echo "running file test in current directory '$PWD', which is mounted on filesystem type: $FS_TYPE"

# cleanup
rm -rf a0 a1 a2

# run the demo
./bug-demo

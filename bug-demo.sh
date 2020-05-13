#!/bin/bash

# Repro code for https://www.virtualbox.org/ticket/8761
# This is a version in bash which is simpler but might call more syscalls
# see the C repro program, bug-demo.c for a version that minimizes syscalls

# set # -x # -e -x

rm -rf a0 a1 a2
mkdir -p a0/subdir a1/subdir


lscmd0='ls -i a0 a0/subdir a0/subdir/file'
lscmd1='ls -i a1 a1/subdir a1/subdir/file'
lscmd2='ls -i a2 a2/subdir a2/subdir/file'


echo HI-A0 >a0/subdir/file
echo HI-A1 >a1/subdir/file


echo "----------- PRETEST STEP 1 ---------------"

$lscmd0
$lscmd1
echo "----------- MOVING 2 ---------------"
mv a1 a2


# uncomment this and it WORKS AGAIN
# comment and it FAILS
# should print HI-A1
# cat a2/subdir/file
echo "----------- AFTER 3 ---------------"
$lscmd0

# this causes the test to WORK, which is... weird.
# ls a2/subdir

echo "----------- MOVE AGAIN 4 ---------------"
mv a0 a1


# $lscmd
echo "----------- REMOVE FILE 5 ---------------"
rm a1/subdir/file

# should print HI-A1
# if this is not found, something went way wrong

echo "----------- THE BIG TEST ---------------"
cat a2/subdir/file
echo "----------- AFTERMATH ---------------"

$lscmd1
$lscmd2

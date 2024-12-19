#!/bin/sh

# Tool to generate heap/stack profiles.
set -ev
valgrind  --tool=massif --massif-out-file="./output/$1.massif.out" "$1"

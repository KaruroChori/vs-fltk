#!/bin/sh

set -ev
valgrind --log-file="./output/$1.valgrind.log" --leak-check=full "$1"

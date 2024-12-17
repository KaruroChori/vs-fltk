#!/bin/sh

set -ev
valgrind --error-exitcode=1 --leak-check=full "$1"

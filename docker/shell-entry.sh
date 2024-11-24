#!/bin/bash

set -e
/docker/main_entry.sh

# For some reason, using simply 'export DISPLAY' doesn't work
su builder -l -c "cd /workspace && DISPLAY=$DISPLAY bash"

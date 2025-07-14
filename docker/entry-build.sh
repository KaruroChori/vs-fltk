#!/bin/bash

set -e
$WORKSPACE/docker/entry-common.sh

# Reminder: changing the double quotes to single will break things
su builder -c "source ~/.bashrc && cd $WORKSPACE && scripts/build-default.sh"

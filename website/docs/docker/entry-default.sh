#!/bin/bash

set -e
$WORKSPACE/docker/entry-common.sh

# If -l, --login is used here, the variables defined in
# docker-compose.yml do not get exported
su builder

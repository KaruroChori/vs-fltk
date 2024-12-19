#!/bin/bash
# entry-common.sh

set -e

if [ -z "$HOSTUID" ]; then
  echo "HOSTUID is not set."
  exit 1
fi

if [ -z "$HOSTGID" ]; then
  echo "HOSTGID is not set."
  exit 1
fi

usermod -u $HOSTUID builder
groupmod -g $HOSTGID builder

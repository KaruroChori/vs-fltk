#!/bin/bash

set -e
/docker/main_entry.sh

su builder -c "DISPLAY=$DISPLAY bash -l -c '\
  cd /workspace && \
  bun install &&
  bun run codegen &&
  bun run meson-setup.clang-release &&
  meson compile -C build/ vs:executable
  '"

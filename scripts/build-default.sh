#!/bin/sh
# This file is part of vs-fltk
# License: https://github.com/KaruroChori/vs-fltk/blob/master/LICENCE.md

set -e

if [ ! -f "src/app/main.cpp" ]; then
  echo "This script must be run from the source root."
  exit 1
fi

set -v

bun install
bun run codegen

if [ "$(uname)" != "Darwin" ]; then
  bun run meson-setup.clang-release
else
  meson setup -Dforce_x11_backend=true --reconfigure build --buildtype=release --native-file toolchains/flatpak.ini
  # Unclear fix to be investigated
  rm subprojects/libtcc/VERSION
fi

meson compile -C build vs:executable

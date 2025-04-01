#!/bin/sh
# This file is part of vs-fltk
# License: https://github.com/KaruroChori/vs-fltk/blob/master/LICENCE.md

set -e

# Ensure the script is run from the source root
if [ ! -f "src/app/main.cpp" ]; then
  echo "This script must be run from the source root."
  exit 1
fi
cmake --version
set -v
if [ "CI" != "true" ]; then
  # Update subprojects no more than every 7 days, or force with '-f'
  CHECK_DATE_FILE="subprojects/.check-date"
  CURRENT_DAY=$(date +%j | awk '{print int($1)}') # Day of the year (001-366)

  if [ "$1" = "-f" ]; then
    echo "Force update requested. Performing subproject update..."
    meson subprojects update --reset
    echo "$CURRENT_DAY" > "$CHECK_DATE_FILE"
  else
    if [ -f "$CHECK_DATE_FILE" ]; then
      LAST_CHECK_DAY=$(awk '{print int($1)}' "$CHECK_DATE_FILE")
      DAYS_DIFF=$((CURRENT_DAY - LAST_CHECK_DAY))

      # Account for year rollover
      if [ $DAYS_DIFF -lt 0 ]; then
        DAYS_DIFF=$((DAYS_DIFF + 365)) # Adjust for non-leap years (simplified for this case)
      fi

      if [ $DAYS_DIFF -ge 7 ]; then
        echo "More than 7 days since last subproject update. Updating..."
        meson subprojects update --reset
        echo "$CURRENT_DAY" > "$CHECK_DATE_FILE"
      else
        echo "Subprojects were updated within the last 7 days. Skipping update."
      fi
    else
      echo "No check-date file found. Performing subproject update..."
      meson subprojects update --reset
      echo "$CURRENT_DAY" > "$CHECK_DATE_FILE"
    fi
  fi
fi

bun install
if [ -d "build" ]; then
  bun run meson-build
else
  bun run codegen
  if [ "$(uname)" != "Darwin" ]; then
    meson setup --prefix=/usr -Db_lto=true -Db_thinlto_cache=true -Db_lto_mode=thin --reconfigure build/ --buildtype=release --native-file platforms/ci-ubuntu.ini
  else
    meson setup -Dforce_x11_backend=false -Db_lto=true -Db_thinlto_cache=true -Db_lto_mode=thin --reconfigure build --buildtype=release --native-file platforms/ci-macos.ini
  fi
fi

meson compile -C build vs:executable

#!/bin/sh
set -eu

root=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
build_dir=${TMPDIR:-/tmp}/keyball-keymap-host-tests
mkdir -p "$build_dir"

clang -std=c11 -Wall -Wextra -Werror \
  -I"$root/tests/host" \
  "$root/tests/host/test_custom_keycodes.c" \
  -o "$build_dir/test_custom_keycodes"
"$build_dir/test_custom_keycodes"

clang -std=c11 -Wall -Wextra -Werror \
  -I"$root/tests/host" \
  "$root/tests/host/test_mouse_speed.c" \
  -o "$build_dir/test_mouse_speed"
"$build_dir/test_mouse_speed"

clang -std=c11 -Wall -Wextra -Werror \
  -I"$root/tests/host" \
  "$root/tests/host/test_auto_mouse_config.c" \
  -o "$build_dir/test_auto_mouse_config"
"$build_dir/test_auto_mouse_config"

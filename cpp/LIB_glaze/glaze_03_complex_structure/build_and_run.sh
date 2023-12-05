#!/bin/bash

set -o errexit
set -o nounset

rm -rf meson_build_dir
meson setup meson_build_dir/ .
meson compile -C meson_build_dir/
meson_build_dir/launcher

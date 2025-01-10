#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


# PRÉREQUIS = avoir buildé la dépendance OTel avec ./setup_otel_dependency.sh

rm -rf NOGIT_build_dir
 CXX=clang++-14 meson setup NOGIT_build_dir/ .
 CXX=clang++-14 meson compile  -C NOGIT_build_dir/
NOGIT_build_dir/bin

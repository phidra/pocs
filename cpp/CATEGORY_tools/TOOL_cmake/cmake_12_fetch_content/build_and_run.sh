#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC show the use of FetchContent to manage dependency of a project.

FetchContent is used to download content at configure-time (in opposition with ExternalProject that happens at build-time).

This POC depends on two things, that we will manage with FetchContent :
    GTFS data, that we must download and unzip
    a library (cppgtfs), that we must download, unpack, PATCH ! and build

Notably : after downloading the library, we can 'add_subdirectory' it if the library has a CMakeLists.txt

FIXME : it is unclear what are the rules that govern the redownload of a contentn.
"""

rm -rf cmake_build_dir

echo ""
echo "===== CONFIGURE (cmake) BEGIN"
cmake -Bcmake_build_dir -H.
echo "===== CONFIGURE (cmake) END"
echo ""


echo ""
echo "===== BUILD (make) BEGIN"
make -C cmake_build_dir
echo "===== BUILD (make) END"
echo ""

echo ""
echo "===== RUN (bin) BEGIN"
chmod +x cmake_build_dir/bin
cmake_build_dir/bin cmake_build_dir/gtfs-sample-data
echo "===== RUN (bin) END"
echo ""

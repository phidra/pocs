#!/bin/bash

set -o errexit
set -o nounset

cat << EOF
This POC show the use of 'add_subdirectory' to gather several CMakeLists.txt


https://cmake.org/cmake/help/v3.19/command/add_subdirectory.html
> The CMakeLists.txt file in the specified source directory will be processed immediately by CMake before processing in the current input file continues beyond this command.

https://cmake.org/pipermail/cmake/2007-November/017897.html
> You would use ADD_SUBDIRECTORY and its sibling SUBDIRS when your
> project has multiple directories. For instance, imagine you have these
> source tree:
> alarmclock
>       |
>       \--- libwakeup
>       \--- clock
> 
> Usually, you'd have three CMakeLists.txt here: one in the 'alarmclock'
> directory (the "root" CMakeLists.txt), one in the 'libwakeup'
> directory and one in the 'clock' directory.
EOF

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
cmake_build_dir/src/leia/leia
cmake_build_dir/src/luke/luke
echo "===== RUN (bin) END"
echo ""

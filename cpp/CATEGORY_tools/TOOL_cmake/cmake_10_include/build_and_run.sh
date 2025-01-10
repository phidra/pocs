#!/bin/bash

set -o errexit
set -o nounset

cat << EOF
This POC show the use of 'include' to gather several CMakeLists.txt

https://cmake.org/cmake/help/v3.19/command/include.html
>  Variable reads and writes access the scope of the caller (dynamic scoping).

As I understand it, it works similarly to C-preprocessor (copy-paste included cmake code in parent)
Notably : paths used in included cmake code will be searched from the parent's context.

https://cmake.org/pipermail/cmake/2007-November/017897.html
> Think of INCLUDE as an #include in C or C++. It is useful when you
> have defined custom commands, custom targets, all your CMakeLists.txt
> have parts in common, etc and you want to write them only once.
> INCLUDE helps you to re-use CMake "code".

EDIT : this POC is thus not a canonical use of include (but is useful nonetheless)

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
cmake_build_dir/leia
cmake_build_dir/luke
echo "===== RUN (bin) END"
echo ""

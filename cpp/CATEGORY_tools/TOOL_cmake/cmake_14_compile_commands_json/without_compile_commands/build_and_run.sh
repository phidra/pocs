#!/bin/bash

set -o errexit
set -o nounset

cat << EOF
This POC shows the generation by cmake of compile_commands.json, which helps ALE to work.

This specific subdir DOES NOT generate compile_commands.json.

Even after the build is done, in the absence of compile_commands.json, when you open src/main.cpp,
an error is displayed on the inclusion of "lib.h" because ALE doesn't know where to find the header.

Indeed, the header is located in the "included" folder, which is passed to gcc by cmake :
without compile_commands.json, ALE has no way to know that.

WARNING : it seems that CMAKE_EXPORT_COMPILE_COMMANDS must be defined AFTER the "project" directive.

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
cmake_build_dir/bin
echo "===== RUN (bin) END"
echo ""

#!/bin/bash

set -o errexit
set -o nounset

cat << EOF
This POC show the use of 'execute_process' to fetch info at configure time.

The path and size of the "ls" executable is fetched and displayed by the compiled app.
NOTE : be careful with strings and quotes...
NOTE : execute_process is resolved at configure time.

cmake -Bcmake_build_dir  -H.

make -C cmake_build_dir

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
chmod +x cmake_build_dir/bin
cmake_build_dir/bin
echo "===== RUN (bin) END"
echo ""

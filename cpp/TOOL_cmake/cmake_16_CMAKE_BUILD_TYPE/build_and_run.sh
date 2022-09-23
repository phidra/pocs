#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF
This POC shows the impact of CMAKE_BUILD_TYPE on passed compiler options.

The focus of this POC is compile-time :

Summary of the command options that cmake uses, according to CMAKE_BUILD_TYPE :

CMAKE_BUILD_TYPE=inexisting-value (or empty) :
<no flags are added>

CMAKE_BUILD_TYPE=Debug :
-g

CMAKE_BUILD_TYPE=RelWithDebInfo :
-O2 -g -NDEBUG

CMAKE_BUILD_TYPE=Release :
-O3 -NDEBUG


So in short :

- if Release -> -NDEBUG -O2/3
- if Debug/DebInfo -> -g

EOF

sleep 1

rm -f NOGIT_*

function compile()
{
    local CMAKE_BUILD_TYPE="$1"
    rm -rf cmake_build_dir
    cmake -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" -Bcmake_build_dir -H.
    make -C cmake_build_dir
    cp cmake_build_dir/compile_commands.json NOGIT_compile_commands_"$CMAKE_BUILD_TYPE".json
}

compile Debug
compile RelWithDebInfo
compile Release
compile Pouet

# with empty variable :
rm -rf cmake_build_dir
cmake -Bcmake_build_dir -H.
make -C cmake_build_dir
cp cmake_build_dir/compile_commands.json NOGIT_compile_commands_EMPTY.json

echo ""
echo ""
echo "Examples of diffs :"
set +o errexit
set -o xtrace
diff NOGIT_compile_commands_Release.json NOGIT_compile_commands_RelWithDebInfo.json
diff NOGIT_compile_commands_Debug.json NOGIT_compile_commands_RelWithDebInfo.json
diff NOGIT_compile_commands_Pouet.json NOGIT_compile_commands_Debug.json

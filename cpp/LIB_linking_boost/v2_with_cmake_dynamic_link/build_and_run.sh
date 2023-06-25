#!/bin/bash

echo ""
echo "===== building"
rm -rf cmake_build_dir
mkdir cmake_build_dir
pushd cmake_build_dir
cmake ..
make
popd

echo ""
echo "===== running"
cmake_build_dir/bin

echo ""
echo "===== ldd bin | grep -i boost"
ldd cmake_build_dir/bin | grep -i boost

echo ""
echo "===== ATTENDU = 'libboost_log'  ET  'libboost_program_options'  dans la sortie de ldd"

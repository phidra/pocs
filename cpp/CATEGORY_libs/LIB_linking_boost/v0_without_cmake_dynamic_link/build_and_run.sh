#!/bin/bash

echo ""
echo "===== building"
rm -f bin
clang++ -std=c++11 -DBOOST_LOG_DYN_LINK main.cpp -lboost_log -lboost_program_options -lpthread -o bin
chmod +x bin

echo ""
echo "===== running"
./bin

echo ""
echo "===== ldd bin | grep -i boost"
ldd bin | grep -i boost

echo ""
echo "===== ATTENDU = 'libboost_log'  ET  'libboost_program_options'  dans la sortie de ldd"

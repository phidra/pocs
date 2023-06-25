#!/bin/bash

echo ""
echo "===== building"
rm -f bin
clang++ -static -UBOOST_LOG_DYN_LINK -std=c++11 main.cpp  -lboost_log -lboost_program_options -lboost_system -lboost_thread  -lpthread -o bin
chmod +x bin

echo ""
echo "===== running"
./bin

echo ""
echo "===== ldd bin | grep -i boost"
ldd bin | grep -i boost

echo ""
echo "===== ATTENDU = aucune librairie boost dans la sortie de ldd"

#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace

echo ""
echo "Cette POC nécessite un g++ suffisamment récent pour pouvoir compiler du C++20 (g++ 11 est OK)"
echo ""

# needs C++20 compliant compiler :
g++ -std=c++20 main.cpp -o NOGIT_bin && ./NOGIT_bin

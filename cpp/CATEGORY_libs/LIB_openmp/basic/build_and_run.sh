#!/bin/bash

set -o errexit
set -o nounset

echo "Building bin"
g++ -std=c++11 -fopenmp main.cpp -o bin
chmod +x bin


echo ""
echo "Running bin"
./bin

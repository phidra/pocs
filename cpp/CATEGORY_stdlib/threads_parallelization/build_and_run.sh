#!/bin/bash

set -o errexit
set -o nounset

clang++ main.cpp -pthread -std=c++11 -o bin
chmod +x bin
./bin

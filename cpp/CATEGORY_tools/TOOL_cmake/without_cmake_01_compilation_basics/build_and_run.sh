#!/bin/bash

set -o errexit
set -o nounset

clang++ main.cpp lib.cpp -o bin
chmod +x bin
./bin

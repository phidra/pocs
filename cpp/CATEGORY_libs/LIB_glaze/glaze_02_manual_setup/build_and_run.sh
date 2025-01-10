#!/bin/bash

set -o errexit
set -o nounset

rm -f NOGIT_bin

# build, using glaze includes :
g++ \
    -std=c++20 \
    -Iglaze_dependency/include \
    src/main.cpp \
    -o NOGIT_bin

# run :
./NOGIT_bin

#!/bin/bash

set -o errexit
set -o nounset

echo """This POC shows that there is a slight overhead in linking
against dynamic libraries (due to GOT/PLT management).

Typical results show a small (~1%) but steady overhead for dynamically linked bin.

"""

echo "Dynamic link :"
clang++ -g -O0 -std=c++11 mylib.cpp -shared -fPIC -o libmylib.so
clang++ -g -O0 -std=c++11 main.cpp -L. -lmylib -o bin_dynamic
LD_LIBRARY_PATH=. ./bin_dynamic

echo "Static link :"
clang++ -O0 -std=c++11 main.cpp mylib.cpp -o bin_static
LD_LIBRARY_PATH=. ./bin_static

#!/bin/bash

set -o errexit
set -o nounset

g++ -I subdir main.cpp subdir/lib.cpp -o bin
chmod +x bin
./bin

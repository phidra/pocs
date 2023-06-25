#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat README.txt

echo ""
echo "====== Building C program"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== run :"
sleep 0.5
./bin.GITIGNORE
sleep 0.5
echo ""

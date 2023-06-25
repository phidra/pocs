#!/bin/bash

set -o errexit
set -o nounset

echo "This POC shows that even for INTERNAL function-calls, a shared lib compiled with PIC has a PLT-indirection"

echo ""
echo "Compiling with and without -fPIC"
clang++ -g -c -O0 mylib.cpp -fno-PIC -o without_pic.o
clang++ -g -O0 mylib.cpp -fPIC -shared -o with_pic.so

echo ""
echo "Now run :"
echo ""
echo "    objdump -M intel -CD with_pic.so | less"
echo "    objdump -M intel -CD without_pic.o | less"
echo ""
echo "And search for 'process' and 'accumulate' symbols : in PIC-code, there is an indirection towards PLT"

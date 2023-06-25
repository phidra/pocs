#!/bin/bash

set -o errexit
set -o nounset


echo """
This POC show the use of dlopen/dlsym.
Somes shared libraries are built into /tmp/ and used *dynamically* by main.
See : https://dwheeler.com/program-library/Program-Library-HOWTO/x172.html
See : https://www.tldp.org/HOWTO/C++-dlopen/index.html

"""

echo "===== building shared libs and main"
rm -f libmul2.so
rm -f libmul10.so
rm -f libadd42.so
rm -f bin
g++ -std=c++11 -shared mul2.cpp -o /tmp/libmul2.so
g++ -std=c++11 -shared mul10.cpp -o /tmp/libmul10.so
g++ -std=c++11 -shared add42.cpp -o /tmp/libadd42.so
g++ -std=c++11 -shared irrelevant.cpp -o /tmp/libirrelevant.so
g++ -std=c++11 main.cpp -ldl -o bin


echo "===== running"
./bin

echo ""
echo "===== EXPECTED = various processing on input number, and their description."

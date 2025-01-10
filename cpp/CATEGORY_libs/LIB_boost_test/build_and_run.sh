#!/bin/bash

echo ""
echo "===== building"
rm -f bin
clang++ -std=c++11 mysupertest.cpp -o bin  # inutile de linker quoi que ce soit, cf. https://www.boost.org/doc/libs/1_66_0/libs/test/doc/html/boost_test/usage_variants.html
chmod +x bin

echo ""
echo "===== running"
./bin

echo ""
echo "===== ATTENDU = exactly 2 tests marked as failed AND we see exactly 3 lines OK, and no line KO"

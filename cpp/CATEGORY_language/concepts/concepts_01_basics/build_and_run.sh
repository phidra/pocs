#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace

echo ""
echo "Cette POC nécessite un g++ suffisamment récent pour pouvoir compiler du C++20 (g++ 11 est OK)"
echo ""

# without concepts, no need to use C++20 :
g++ main_without_concepts.cpp -o NOGIT_bin_without && ./NOGIT_bin_without

# with concepts, needs C+20 :
g++ -std=c++20 main_with_concepts.cpp -o NOGIT_bin_with && ./NOGIT_bin_with

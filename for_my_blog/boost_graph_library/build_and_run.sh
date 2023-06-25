#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

echo ""
echo "===== building"
rm -f NOGIT_bin
clang++ -std=c++14 -Wall dijkstra.cpp -o NOGIT_bin_dijkstra
clang++ -std=c++14 -Wall oldproperties.cpp -o NOGIT_bin_oldproperties
chmod +x NOGIT_bin_dijkstra NOGIT_bin_oldproperties

echo ""
echo "===== running"
./NOGIT_bin_dijkstra
./NOGIT_bin_oldproperties

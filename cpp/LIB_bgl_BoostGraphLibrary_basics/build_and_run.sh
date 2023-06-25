#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

pocs="${1:-"$(ls poc*.cpp)"}"


echo "Building and running pocs : ${pocs}"

echo ""
for poc in ${pocs}
do
    bin="bin_${poc%%.cpp}"
    echo "===== building ${poc}"
    rm -f $bin
    clang++ -std=c++11 -Wall ${poc} -o $bin
    chmod +x $bin
done

for poc in ${pocs}
do
    bin="bin_${poc%%.cpp}"
    echo ""
    echo "===== running ${bin}"
    ./${bin}
done

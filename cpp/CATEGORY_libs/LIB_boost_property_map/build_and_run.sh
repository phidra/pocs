#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF


===== IMPORTANT : il y a beaucoup de notes en commentaire de main_basics.cpp / main_iterator_property_map.cpp
EOF

pocs="${1:-basics iterator_property_map}"

for poc in ${pocs}
do
    echo "===== building ${poc}"
    rm -f bin_${poc}
    clang++ -std=c++11 -Wall main_${poc}.cpp -o bin_${poc}
    chmod +x bin_${poc}
done

for poc in ${pocs}
do
    echo ""
    echo "===== running ${poc}"
    ./bin_${poc}
done

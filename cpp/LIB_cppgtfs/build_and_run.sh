#!/bin/zsh

set -o errexit
set -o nounset
set -o pipefail


cat << EOF


===== POC d'utilsiation de la librairie cppgtfs :
https://ad-git.informatik.uni-freiburg.de/ad/cppgtfs

Copie de la librairie :
wget https://ad-git.informatik.uni-freiburg.de/ad/cppgtfs/-/archive/master/cppgtfs-master.zip
unzip cppgtfs-master.zip
mv cppgtfs-master/src cppgtfs
rm -rf cppgtfs-master cppgtfs-master.zip

Le sample GTFS a été téléchargé ici :
https://github.com/google/transit/raw/master/gtfs/spec/en/examples/sample-feed-1.zip

EOF

# echo "===== building"
# rm -f bin
# clang++ -std=c++14 -Wall -Icppgtfs cppgtfs/**/*.cpp main.cpp -o bin
# chmod +x bin

# echo ""
# echo "===== running"
# ./bin gtfs_sample

BUILD_DIR="_build"
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake .. && make

cd ..
"${BUILD_DIR}/bin" "gtfs_sample"

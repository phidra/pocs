#!/bin/bash

set -o errexit
set -o nounset

echo ""
echo "===== building"
rm -f bin
rm -rf data_GITIGNORE
mkdir data_GITIGNORE
clang++ -std=c++11 -Wall main_boost_iostream.cpp -lboost_iostreams -DBOOST_IOSTREAMS_NO_LIB -lz -o bin         
chmod +x bin


echo ""
echo "===== creating gzipped content :"
echo "[ALREADY COMPRESSED]  Kenzo Tenma" > data_GITIGNORE/already_compressed
echo "[ALREADY COMPRESSED]  Johann" >> data_GITIGNORE/already_compressed
gzip data_GITIGNORE/already_compressed

echo ""
echo "===== running"
./bin

echo ""
echo "===== contenu de log1.txt et log2.txt :"
cat data_GITIGNORE/log1.txt
cat data_GITIGNORE/log2.txt

echo ""
echo "===== contenu de output.gz :"
zcat data_GITIGNORE/output.gz

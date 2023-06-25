#!/bin/bash

echo "================================================================================"
echo "=== Mise au propre, puis compression avec python :"
echo "================================================================================"
rm -f dst_*
rm -rf poc_check_dir
./compress_files.py


echo "================================================================================"
echo "=== Contenu de la tarball non-compressée :"
echo "================================================================================"
cat dst_uncompressed.tar


echo "================================================================================"
echo "=== Contenu de la tarball compressée :"
echo "================================================================================"
cat dst_compressed.tar.gz
echo ""


echo "================================================================================"
echo "=== Décompression de la tarball compressée dans le répertoire \"poc_check_dir\" :"
echo "================================================================================"
mkdir poc_check_dir
tar -xvzf dst_compressed.tar.gz --directory poc_check_dir


echo "================================================================================"
echo "=== Contenu du répertoire :"
echo "================================================================================"
tree -A -C -N -L 1 poc_check_dir


echo "================================================================================"
echo "=== Les fichiers sont ils identiques :"
echo "================================================================================"
md5sum ./file_to_compress_1 poc_check_dir/file_to_compress_1
echo ""
md5sum ./file_to_compress_2 poc_check_dir/file_to_compress_2
echo ""
md5sum ./file_to_compress_3 poc_check_dir/file_to_compress_3

#!/bin/bash

set -o errexit
set -o nounset


cat README.txt

TEMPFILE="$(mktemp)" || exit 1
trap 'rm -f "$TEMPFILE"' EXIT
NB_OF_LOOPS=200
head -c 1M < /dev/urandom > "$TEMPFILE"

echo ""
echo "Building C program :"
gcc main.c -o bin.GITIGNORE

echo ""
echo "Running tests :"
SECTOR_SIZE=$(cat /sys/block/sda/queue/physical_block_size)
CHUNK_SMALLER=$((SECTOR_SIZE - 2))
CHUNK_BIGGER=$((SECTOR_SIZE + 2))
echo "SECTOR SIZE = $SECTOR_SIZE"
echo "Trying with chunk_size = $CHUNK_SMALLER / $CHUNK_BIGGER"

echo ""
echo "====== CASE 1 : with chunk_size SMALLER than sector size :"
/usr/bin/time -p ./bin.GITIGNORE $TEMPFILE $CHUNK_SMALLER  $NB_OF_LOOPS
/usr/bin/time -p ./bin.GITIGNORE $TEMPFILE $CHUNK_SMALLER  $NB_OF_LOOPS
/usr/bin/time -p ./bin.GITIGNORE $TEMPFILE $CHUNK_SMALLER  $NB_OF_LOOPS


echo ""
echo "====== CASE 2 : with chunk_size BIGGER than sector size :"
/usr/bin/time -p ./bin.GITIGNORE $TEMPFILE $CHUNK_BIGGER   $NB_OF_LOOPS
/usr/bin/time -p ./bin.GITIGNORE $TEMPFILE $CHUNK_BIGGER   $NB_OF_LOOPS
/usr/bin/time -p ./bin.GITIGNORE $TEMPFILE $CHUNK_BIGGER   $NB_OF_LOOPS

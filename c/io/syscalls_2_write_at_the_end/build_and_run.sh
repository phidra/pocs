#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TMPFILE="$(mktemp)" || exit 1
trap 'rm -f "$TMPFILE"' EXIT

cp poeme.txt "$TMPFILE"

echo ""
echo "====== Building C program :"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== BEFORE run, content of TMPFILE $TMPFILE  :"
echo ""
sleep 1
cat "$TMPFILE"
sleep 2


echo ""
echo "====== RUNNING (thus editing the file) :"
echo ""
./bin.GITIGNORE "$TMPFILE"


echo ""
echo "====== AFTER run, content of TMPFILE $TMPFILE  :"
echo ""
sleep 1
cat "$TMPFILE"
echo ""
sleep 2

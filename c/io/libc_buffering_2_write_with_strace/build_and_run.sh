#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TEMPFILE="$(mktemp)" || exit 1
trap 'rm -f "$TEMPFILE"' EXIT

echo ""
echo "Building C program :"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== strace on bin :"
sleep 1
echo ""
strace ./bin.GITIGNORE "$TEMPFILE" 2>&1 | grep -e "nanosleep(" -e "write("
sleep 1


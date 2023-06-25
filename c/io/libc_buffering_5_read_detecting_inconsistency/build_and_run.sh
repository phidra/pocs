#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TMPFILE="$(mktemp)" || exit 1
RESULTFILE=results.GITIGNORE
trap 'rm -f "$TMPFILE" "$RESULTFILE"' EXIT

cp poeme.txt "$TMPFILE"

echo ""
echo "====== Building C program :"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== FIRST CASE = chunk by chunk copy, identical :"
echo ""
sleep 1
rm -f "$RESULTFILE"
strace ./bin.GITIGNORE "$TMPFILE" "$RESULTFILE" 2>&1 | grep -e "nanosleep(" -e "read("
echo ""
sleep 3

echo "====== Comparing files..."
du -sb "$TMPFILE" "$RESULTFILE"
cmp "$TMPFILE" "$RESULTFILE"  # will fail if different
echo "The files are identical"


echo ""
echo "====== SECOND CASE = chunk by chunk copy, with an addition at the end :"
sleep 2
rm -f "$RESULTFILE"
strace ./bin.GITIGNORE "$TMPFILE" "$RESULTFILE" 2>&1 | grep -e "nanosleep(" -e "read(" &


sleep 5
echo "====== I WILL NOW WRITE A SMALL ADDITION TO THE FILE : $TMPFILE"
echo "pouet" >> "$TMPFILE"

while [ ! -e "$RESULTFILE" ] ; do sleep 1 ; done  # hard to wait strace pid with my piping :-(
echo ""
sleep 2


echo ""
echo "====== Comparing files..."
du -sb "$TMPFILE" "$RESULTFILE"
cmp "$TMPFILE" "$RESULTFILE"  # will fail if different
echo "The files are (a bit suprisingly) identical"

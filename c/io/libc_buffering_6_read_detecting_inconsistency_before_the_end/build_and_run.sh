#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TMPFILE="$(mktemp)" || exit 1
RESULTFILE=results.GITIGNORE
trap 'rm -f "$TMPFILE" "$RESULTFILE"' EXIT

cp poeme.txt "$TMPFILE"

echo ""
echo "====== Building C programs :"
gcc main.c -o bin.GITIGNORE
gcc write_before_the_end.c -o write_before_the_end.GITIGNORE

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
echo "====== SECOND CASE = chunk by chunk copy, with an addition few bytes before the end :"
sleep 2
rm -f "$RESULTFILE"
strace ./bin.GITIGNORE "$TMPFILE" "$RESULTFILE" 2>&1 | grep -e "nanosleep(" -e "read(" &


sleep 5
echo "====== I WILL NOW WRITE A SMALL ADDITION FEW BYTES BEFORE THE END OF THE FILE : $TMPFILE"
MODIFCATION="===COUCOU===" 
./write_before_the_end.GITIGNORE "$TMPFILE" "$MODIFCATION" > /dev/null
grep -q "$MODIFCATION" "$TMPFILE"  # will fail if string not in the file

while [ ! -e "$RESULTFILE" ] ; do sleep 1 ; done  # hard to wait strace pid with my piping :-(
echo ""
sleep 2


echo ""
echo "====== Content of the (modified) source file :"
cat "$TMPFILE"
echo ""

echo ""
echo "====== Content of the result file :"
cat "$RESULTFILE"
echo ""

echo ""
echo "====== Comparing files (will fail, because they are different) :"
du -sb "$TMPFILE" "$RESULTFILE"
cmp "$TMPFILE" "$RESULTFILE"  # will fail because different !
echo "If we arrive here, The files are identical (but that should not be the case...)"

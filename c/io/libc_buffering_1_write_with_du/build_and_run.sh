#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TEMPFILE1="$(mktemp)" || exit 1
TEMPFILE2="$(mktemp)" || exit 2
trap 'rm -f "$TEMPFILE1" ; rm -f "$TEMPFILE2"' EXIT

echo ""
echo "Building C programs :"
gcc main1_content_smaller_than_buffer.c -o bin1.GITIGNORE
gcc main2_content_bigger_than_buffer.c -o bin2.GITIGNORE

echo ""
echo "====== CASE 1 : writing content that is smaller than libc's buffer :"
./bin1.GITIGNORE "$TEMPFILE1" &
PID1=$!

echo ""
echo "Displaying written file size in bytes :"
for i in {1..10}
do
    sleep 0.8
    echo "Size seen by 'du -sb' is : $(du -sb $TEMPFILE1)"
done

echo ""

wait $PID1
sleep 3

echo ""
echo "====== CASE 2 : writing content that is bigger than libc's buffer :"
./bin2.GITIGNORE "$TEMPFILE2" &
PID2=$!

echo ""
echo "Displaying written file size in bytes :"
for i in {1..10}
do
    sleep 0.8
    echo "Size seen by 'du -sb' is : $(du -sb $TEMPFILE2)"
done

echo ""

wait $PID2
sleep 1


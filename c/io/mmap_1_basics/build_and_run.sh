#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TMPSMALL="$(mktemp --tmpdir -- 'POC_SMALL.XXXX')" || exit 1
TMPMEDIUM="$(mktemp --tmpdir -- 'POC_MEDIUM.XXXX')" || exit 2
TMPBIG="$(mktemp --tmpdir -- 'POC_BIG.XXXX')" || exit 3
trap 'rm -f "$TMPSMALL" "$TMPMEDIUM" "$TMPBIG"' EXIT

# ~256 bytes files (270 bytes) :
cat << EOF > "$TMPSMALL"
La cigale ayant chante tout l'ete,
Se trouva fort depourvue quand la bise fut venue.
Pas un seul petit morceau de mouche ou de vermisseau.
Elle alla crier famine chez la Fourmi sa voisine,
La priant de lui preter quelque grain pour subsister
Jusqu'a la saison nouvelle.
EOF
cat "$TMPSMALL" "$TMPSMALL" "$TMPSMALL" "$TMPSMALL" > "$TMPMEDIUM"  # ~1k
for i in $(seq 1 16) ; do cat "$TMPMEDIUM" >> "$TMPBIG" ; done  # ~16k


echo ""
echo "====== Building C program"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== run outside of strace to see output :"
./bin.GITIGNORE "$TMPBIG"
sleep 1
echo ""

echo ""
echo "====== now, strace on bin :"
sleep 1
echo ""
strace ./bin.GITIGNORE "$TMPBIG" 2>&1 | grep -e "nanosleep(" -e "read(" -e "mmap"
sleep 1


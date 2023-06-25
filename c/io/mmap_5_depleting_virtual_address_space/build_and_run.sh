#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat README.txt

TMPSMALL="FILE_small.NOGIT"
TMPMEDIUM="FILE_medium.NOGIT"
TMPBIG="FILE_big.NOGIT"
TMPPRETTYBIG="FILE_prettybig.NOGIT"

# ~256 bytes files (270 bytes) :
echo ""
echo "====== Building temporary files."
cat << EOF > "$TMPSMALL"
La cigale ayant chante tout l'ete,
Se trouva fort depourvue quand la bise fut venue.
Pas un seul petit morceau de mouche ou de vermisseau.
Elle alla crier famine chez la Fourmi sa voisine,
La priant de lui preter quelque grain pour subsister
Jusqu'a la saison nouvelle.
EOF
cat "$TMPSMALL" "$TMPSMALL" "$TMPSMALL" > "$TMPMEDIUM"  # ~1k
rm -f "$TMPBIG" "$TMPPRETTYBIG"
for i in $(seq 1 1024) ; do cat "$TMPMEDIUM" >> "$TMPBIG" ; done  # ~1 Mio
for i in $(seq 1 130) ; do cat "$TMPBIG" >> "$TMPPRETTYBIG" ; done  # ~100 Mio


echo ""
echo "====== Building C program"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== run :"
sleep 0.5
./bin.GITIGNORE "$TMPPRETTYBIG"
sleep 0.5
echo ""

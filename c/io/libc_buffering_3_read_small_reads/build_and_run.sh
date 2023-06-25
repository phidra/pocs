#!/bin/bash

set -o errexit
set -o nounset

cat README.txt

TMPFILE="$(mktemp)" || exit 1
trap 'rm -f "$TMPFILE"' EXIT

# ~256 bytes files (270 bytes) :
cat << EOF > "$TMPFILE"
La cigale ayant chante tout l'ete,
Se trouva fort depourvue quand la bise fut venue.
Pas un seul petit morceau de mouche ou de vermisseau.
Elle alla crier famine chez la Fourmi sa voisine,
La priant de lui preter quelque grain pour subsister
Jusqu'a la saison nouvelle.
EOF

echo ""
echo "Building C program :"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== strace on bin :"
sleep 1
echo ""
strace ./bin.GITIGNORE "$TMPFILE"
# strace ./bin.GITIGNORE "$TMPFILE" 2>&1 | grep -e "nanosleep(" -e "read("
sleep 1


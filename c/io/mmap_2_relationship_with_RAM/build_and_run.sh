#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

function display_free_mem()
{
    local sleep_duration=$1
    local current_iteration=0
    while true
    do
        freemem_gb="$(echo "$(grep MemFree /proc/meminfo|cut -d" " -f 10) / 1024" | bc)"
        echo "$freemem_gb Mb"
        sleep "$sleep_duration"
    done
}

cat README.txt

TMPSMALL="FILE_small.NOGIT"
TMPMEDIUM="FILE_medium.NOGIT"
TMPBIG="FILE_big.NOGIT"
TMPVERYBIG="FILE_verybig.NOGIT"

if [ ! -f "$TMPSMALL" -o ! -f "$TMPMEDIUM" -o ! -f "$TMPBIG" -o ! -f "$TMPVERYBIG" ]
then
    # ~256 bytes files (270 bytes) :
    echo "Building temporary files."
    cat << EOF > "$TMPSMALL"
    La cigale ayant chante tout l'ete,
    Se trouva fort depourvue quand la bise fut venue.
    Pas un seul petit morceau de mouche ou de vermisseau.
    Elle alla crier famine chez la Fourmi sa voisine,
    La priant de lui preter quelque grain pour subsister
    Jusqu'a la saison nouvelle.
EOF
    cat "$TMPSMALL" "$TMPSMALL" "$TMPSMALL" > "$TMPMEDIUM"  # ~1k
    for i in $(seq 1 1024) ; do cat "$TMPMEDIUM" >> "$TMPBIG" ; done  # ~1 Mio
    for i in $(seq 1 1024) ; do cat "$TMPBIG" >> "$TMPVERYBIG" ; done  # ~1 Gio
else
    echo "Using already built temporary files."
fi



echo ""
echo "====== Building C program"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== If you want to clear page-cache, run this command as root in another terminal :"
echo ""
echo "      free -mhw && sync && echo 1 > /proc/sys/vm/drop_caches && free -mhw"
echo ""
echo "(press Enter when you're good)"
read


echo ""
echo "====== Regularly displaying free mem"
display_free_mem 0.4&
free_mem_display_pid=$!

echo ""
echo "====== run :"
sleep 1
./bin.GITIGNORE "$TMPVERYBIG"
sleep 1
echo ""

kill $free_mem_display_pid

wait $free_mem_display_pid

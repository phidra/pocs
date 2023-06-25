#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

function call()
{
    echo "===== CALLING :   $@"
    eval "$@"
}

echo ""
echo "===== Préparation d'un fichier de données :"
trap 'rm -f "$DATAFILE"' EXIT
DATAFILE=$(mktemp --suffix "_DATAFILE_FOR_POC")

cat << EOF > "$DATAFILE"
Anakin
Luke
Leia
EOF


echo ""
echo "===== Contenu du fichier de données :"
echo "cat $DATAFILE"
cat $DATAFILE


echo ""
echo "===== De base, le fichier n'est pas accessible depuis le container :"
call docker run -it bash:latest -c \"cat\ /tmp/path/to/a/folder/skywalker.txt\" || true

echo ""
echo "===== On peut monter un fichier unique comme un volume dans le container :"
call docker run -it -v "$DATAFILE":/tmp/path/to/a/folder/skywalker.txt bash:latest -c \"cat\ /tmp/path/to/a/folder/skywalker.txt\"

#!/bin/bash

set -o errexit  # nécessaire pour que la syntax error exite

trap "echo 'TRAPPED : appel du trap, alors même qu-on a exit à cause d-une erreur de syntaxe'" EXIT

echo "Juste après cette ligne, on va provoquer une erreur de syntaxe..."
poujsdjksdglksdgnkldsngklsngklsdnlgkdsnkglsnkl
echo "Cette ligne ne sera jamais exécutée."

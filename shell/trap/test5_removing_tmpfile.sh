#!/bin/bash

# NOTE : au moment de la définition du trap, la variable TMPFILE n'est pas expand grâce aux simples quotes
trap 'echo "TRAPPED : on va supprimer le TMPFILE..." ; rm -f "$TMPFILE" && echo "TRAPPED : ... done"' EXIT

echo "Création d'un tmpfile :"
TMPFILE=$(mktemp) || exit 1
echo "Le temporary file est : $TMPFILE"
echo "On s'apprête à exit 0, ce qui triggera le trap, et effacera le tmpfile."
echo "Vérifiez-le en lançant :"
echo ""
echo "    ls -lh $TMPFILE"
echo ""
exit 0
echo "Cette ligne ne sera jamais exécutée."

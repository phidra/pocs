#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de set -e pour sortir en erreur à la première commande qui échoue.
#
# Montrer que le set -e n'empêche pas les conditions de fonctionner (en ne sortant pas DÈS que la condition sort en erreur).
########################################################################################################################

echo "=== Activation du mode -e"
set -e
echo ""


false || echo "=== Une commande vient d'échouer, mais comme elle est dans une condition, elle n'arrête pas l'exécution du script."
echo ""

echo "=== On va lancer la même commande en dehors de la condition, et elle fera arrêter le script :"
false
echo ""


echo "=== SUITE DU SCRIPT → ON NE DEVRAIT JAMAIS PASSER PAR ICI !"
echo ""

########################################################################################################################
# Fin du fichier.
########################################################################################################################

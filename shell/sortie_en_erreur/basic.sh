#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de set -e pour sortir en erreur à la première commande qui échoue.
#
# Démonstration du principe = arrêter l'exécution à la première erreur rencontrée.
########################################################################################################################


echo "=== La commande suivante va provoquer en erreur, puis le script se poursuivra :"
echo "Appel de \"false\"..." && false

echo ""
echo "=== SUITE DU SCRIPT (après une erreur)"
echo ""

echo "=== Activation du mode -e"
set -e
echo ""

echo "=== La commande suivante va provoquer en erreur, mais comme set -e est mis, le script s'arrêtera (alors qu'il peut continuer) :"
echo "Appel de \"false\"..." && false

echo ""
echo "=== SUITE DU SCRIPT (après une erreur)"
echo ""

########################################################################################################################
# Fin du fichier.
########################################################################################################################

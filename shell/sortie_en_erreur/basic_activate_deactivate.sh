#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de set -e pour sortir en erreur à la première commande qui échoue.
#
# Montrer qu'on peut jongler avec ce mode pour autoriser certaines commandes à sortir en erreur.
########################################################################################################################


echo "=== La commande suivante va provoquer en erreur (car elle n'existe pas), puis le script se poursuivra :"
echo "Appel de \"false\"..." && false

echo ""
echo "=== SUITE DU SCRIPT (après une erreur)"
echo ""

echo "=== Activation du mode -e"
set -e
echo ""

echo "=== Les commandes suivantes ne provoqueront pas d'erreur (sinon le script s'arrêterait) :"
echo "Appel de \"true\"..." && true
echo "Appel de \"true\"..." && true
echo "Appel de \"true\"..." && true

echo "=== Désactivation du mode -e"
set +e
echo ""

echo "=== Les commandes suivantes peuvent provoquer une erreur sans arrêter le script (puisque -e est désactivé) :"
echo "Appel de \"false\"..." && false
echo "Appel de \"false\"..." && false
echo "Appel de \"false\"..." && false
echo "Appel de \"commande inexistante\"..." && pouetpouet
echo "Appel de \"commande inexistante\"..." && pouetpouet
echo "Appel de \"commande inexistante\"..." && pouetpouet

echo "=== Réactivation du mode -e"
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

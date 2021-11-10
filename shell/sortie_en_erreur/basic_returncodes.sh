#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de set -e pour sortir en erreur à la première commande qui échoue.
#
# Codes d'erreur renvoyés si le script s'arrête.
#
# (pour démontrer ces codes, on lance des sous-shells (entre parenthèse) qui sortent en erreur, et on en affiche le code.
########################################################################################################################


echo "=== Lancement d'un sous-shell en mode -e qui n'échoue pas :"
(
    set -e
    echo "    * lancement d'une commande \"true\" dans le sous-shell..."
    true
)
echo "=== Il est sorti avec le returncode : $?"
echo ""

###

echo "=== Lancement d'un sous-shell en mode -e qui provoque une erreur à cause d'une commande inexistante :"
(
    set -e
    echo "    * lancement d'une commande inexistante dans le sous-shell..."
    pouetpouetcamembert
    echo "    * suite du sous-shell (ON NE DEVRAIT JAMAIS AFFICHER CECI)"
)
echo "=== Il est sorti avec le returncode : $?"
echo ""

###

echo "=== Lancement d'un sous-shell en mode -e qui sort en erreur :"
(
    set -e
    echo "    * lancement d'un sous-shell qui provoque une erreur à cause d'une fonction qui renvoie 42..."
    boum () { return 42; }
    boum   
    echo "    * suite du sous-shell (ON NE DEVRAIT JAMAIS AFFICHER CECI)"
)
echo "=== Il est sorti avec le returncode : $?"
echo ""

###

echo "=== Lancement d'un sous-shell en mode -e qui sort en erreur :"
(
    set -e
    echo "    * lancement d'un sous-shell qui provoque une erreur à cause d'une commande qui renvoie 19.."
    ./exit_with_returncode.sh 19
    echo "    * suite du sous-shell (ON NE DEVRAIT JAMAIS AFFICHER CECI)"
)
echo "=== Il est sorti avec le returncode : $?"
echo ""

###

echo "=== Lancement d'un sous-shell SANS mode -e qui sort en erreur :"
(
    set +e
    echo "    * lancement d'un sous-shell qui provoque une erreur à cause d'une commande qui renvoie 37..."
    ./exit_with_returncode.sh 37
    echo "    * suite du sous-shell (qui montre que le sous-shell n'a pas arrêté son exécution après une erreur rencontrée)"
)
echo "=== Il est sorti avec le returncode : $?"
echo ""

########################################################################################################################
# Fin du fichier.
########################################################################################################################


#!/bin/bash
########################################################################################################################
# Script utilitaire pour la bashpoc d'utilisation de set -e.
#
# Sort en erreur avec le returncode passé en premier argument ($1), par défaut : 42.
########################################################################################################################

returncode=42
[ "$1" != "" ] && returncode="$1"
exit "${returncode}"

########################################################################################################################
# Fin du fichier.
########################################################################################################################

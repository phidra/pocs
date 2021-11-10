#!/bin/bash
########################################################################################################################
# POC d'utilisation de rsync pour faire un miroir à l'identique d'un répertoire, et notamment de synchroniser :
#    - des ajouts de fichiers
#    - des renommages de fichiers
#    - des SUPPRESSIONS de fichiers
#
# C'est surtout la synchronisation de la suppression de fichiers qui est intéressante.
#
# Conclusion de la POC : la commande suivante permet bien de répercuter sur la target TOUTE modification de la source :
#
#      rsync -avz --delete SOURCE TARGET
#
########################################################################################################################

TEMPLATE="dir.template"
SOURCE="source_NOGIT"
TARGET="target_NOGIT"
RSYNC_COMMAND="rsync -avz --delete ${SOURCE} ${TARGET}"

########################################################################################################################
# Fonctions utilitaires :
########################################################################################################################

function display_dir()
{
    local dir="$1"
    echo ""
    echo "=== DIRECTORY : $dir"
    find "$1" -type f | while read -r f
    do
        echo "    +++ FILE : ${f}"
        while read -r line
        do
            echo "        ${line}"
        done < "${f}"
        echo ""
    done
}

function dirhash()
{
    find "$1" -type f -print0 | sort -z | xargs -0 cat | md5sum
}

function log()
{
    echo ""
    echo "=== $*"
}

function logstep()
{
    echo ""
    echo "================================================================================"
    echo "=== $*"
    echo "================================================================================"
}

function loglaunch()
{
    echo "=== RUNNING : $*"
    eval "$@"
}

function compare_source_and_target()
{
    echo ""
    echo "=== À ce stade, les répertoires source et target sont-ils identiques ?"
    echo "        DIRHASH(${SOURCE}) = $(dirhash ${SOURCE})"
    echo "        DIRHASH(${TARGET}) = $(dirhash ${TARGET})"
}

########################################################################################################################
# Préparation :
########################################################################################################################

# Mise au propre :
rm -rf "${SOURCE}" "${TARGET}"

# Création du répertoire source :
cp -R "${TEMPLATE}" "${SOURCE}"

################################################################################

logstep "Explication de la POC et des étapes"
log """
=== OBJECTIF :
    L'objectif de la POC est de tester la commande permettant de mirrorer un répertoire source vers un répertoire target.
    La commande doit garder le contenu de la target correctement synchronisé, malgré des modifications sur le contenu de la source.
    Voici la commande testée :

        ${RSYNC_COMMAND}

=== ÉTAPES :
    ÉTAPE 1. on part d'un répertoire source contenant divers fichiers, que l'on affiche.
    ÉTAPE 2. via la commande, on créé la target (comme clone de la source), et on vérifie que les deux répertoires sont bien identiques
    ÉTAPE 3. on fait et affiche toutes les modifications intéressantes sur la source (i.e. celles dont on veut tester l'impact sur la synchronisation)
    ÉTAPE 4. on appelle la commande de synchronisation, et on vérifie que les deux répertoires sont bien identiques
"""

################################################################################

logstep "ÉTAPE 1. on part d'un répertoire source contenant divers fichiers, que l'on affiche."
display_dir "${SOURCE}"

################################################################################

logstep "ÉTAPE 2. via la commande, on créé la target (comme clone de la source), et on vérifie que les deux répertoires sont bien identiques"
loglaunch "${RSYNC_COMMAND}"
display_dir "${TARGET}"
compare_source_and_target

################################################################################

logstep "ÉTAPE 3. on fait et affiche toutes les modifications intéressantes sur la source (i.e. celles dont on veut tester l'impact sur la synchronisation)"

log "Ajout."
loglaunch 'echo "Kit" > '"${SOURCE}/Fisto"
log "Suppression."
loglaunch rm "${SOURCE}/Katarn"
log "Déplacement."
loglaunch mv "${SOURCE}/Skywalker" "${SOURCE}/TheSkywalkers"
log "Modification."
loglaunch 'echo "Samuel L. Jackson" >> '"${SOURCE}/Windu"
log "Fichier non-modifié = Solo"

display_dir "${SOURCE}"
compare_source_and_target

################################################################################

logstep "ÉTAPE 4. on appelle la commande de synchronisation, et on vérifie que les deux répertoires sont bien identiques"
loglaunch "${RSYNC_COMMAND}"
display_dir "${TARGET}"
compare_source_and_target

########################################################################################################################
# Fin du fichier.
########################################################################################################################

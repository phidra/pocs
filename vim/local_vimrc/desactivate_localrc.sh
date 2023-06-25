#!/bin/bash
########################################################################################################################

# Chemin d'appel du script :
script_dir="$(dirname $0)"
root_dir="$(realpath $script_dir)"

# Projets :
PROJECT1="${root_dir}/project1"
PROJECT2="${root_dir}/project2"

# Fichiers de tags :
TAGFILE1="project1_tags"
TAGFILE2="dasupertags"

# Fichier local vim :
VIMLOCAL="_vimrc_local.vim"

########################################################################################################################
# MAIN :
########################################################################################################################

# On supprime les tagfiles s'ils existent :
rm -f "${PROJECT1}/${TAGFILE1}"
rm -f "${PROJECT2}/${TAGFILE2}"

# On supprime les scripts locaux vim s'ils existent :
rm -f "${PROJECT1}/${VIMLOCAL}"
rm -f "${PROJECT2}/${VIMLOCAL}"

# On affiche les instructions :
echo """
La POC est maintenant déconfigurée (elle ne charge plus automatiquement les bons fichiers de tags).
Pour le vérifier :

    - charger un fichier main.py de l'un des deux projets
        cd project1 && vim main.py
    - essayer d'utiliser la navigation par tag pour rejoindre la librairie (Ctrl+clic-gauche sur un nom importé)
    - constater que ça ne fonctionne pas :-(

"""
########################################################################################################################
#Fin du fichier.
########################################################################################################################


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

# On créé les tagfiles :
pushd "${PROJECT1}" 1> /dev/null && ctags -R --languages=python -f "${TAGFILE1}" && popd 1> /dev/null
pushd "${PROJECT2}" 1> /dev/null && ctags -R --languages=python -f "${TAGFILE2}" && popd 1> /dev/null

# On crée les scripts locaux vim :
echo ':set tags=project1_tags;/' > "${PROJECT1}/${VIMLOCAL}"
echo ':set tags=dasupertags;/' > "${PROJECT2}/${VIMLOCAL}"

# On affiche les instructions :
echo """
La POC est maintenant configurée pour charger automatiquement les bons fichiers de tags.
Pour le vérifier :

    - charger un fichier main.py de l'un des deux projets
        cd project1 && vim main.py
    - utiliser la navigation par tag pour rejoindre la librairie (Ctrl+clic-gauche sur un nom importé)
    - constater que ça fonctionne :-)

"""

########################################################################################################################
#Fin du fichier.
########################################################################################################################

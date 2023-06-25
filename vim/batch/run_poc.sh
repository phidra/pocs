#!/bin/bash
########################################################################################################################
# Exécution de la POC de modification de fichiers par vim en mode batch.
#
# Description de la POC :
#    Il est possible d'utiliser vim comme un exécutable de modification de texte (non-interactif).
#    Ça revient à batcher des commandes vim, ce qui peut-être très pratique, notamment pour supprimer des lignes.
#    Attention, ça ne marche pas si le fichier édité est déjà ouvert par vim par ailleurs !
#
########################################################################################################################

to_execute='vim -e input.txt < vim_commands'
outputfile="output.GITIGNORE"

rm -f "${outputfile}"

echo 'About to execute :'
echo ''
echo "    ${to_execute}"
echo ''

eval "${to_execute}"
echo 'Done !'
echo ''

echo ''
echo 'Content of output file :'
cat "${outputfile}"
echo ''

########################################################################################################################
# Fin du fichier.
########################################################################################################################

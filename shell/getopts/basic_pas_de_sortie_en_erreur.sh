#!/bin/bash
########################################################################################################################
# POC sur l'utilisation de getopts pour parser des arguments.
#
# Démonstration que getopts ne sort pas de lui-même en cas d'erreur : il laisse l'utilisateur décider quoi faire.
#
# Les appels montrant la POC :
#    ./basic_pas_de_sortie_en_erreur.sh            (cas ne provoquant pas d'erreur, normal)
#    ./basic_pas_de_sortie_en_erreur.sh -a         (cas ne provoquant pas d'erreur, normal)
#    ./basic_pas_de_sortie_en_erreur.sh -b POUET   (cas ne provoquant pas d'erreur, normal)
#    ./basic_pas_de_sortie_en_erreur.sh -b         (cas PROVOQUANT une erreur, mais sans interrompre le script)
#    ./basic_pas_de_sortie_en_erreur.sh -c         (cas PROVOQUANT une erreur, mais sans interrompre le script)
# 
########################################################################################################################

while getopts "ab:" opt # si on ne précise pas de troisième argument à getopt, il utilise par défaut "$@"
do
    case $opt in
    a)
        echo "Le flag  '-a'  a été passé en option !"
    ;;
    b)
        echo "Le flag  '-b'  a été passé en option, avec l'argument \"$OPTARG\" !"
    ;;
    \?) # Si getopt rencontre une erreur, il settera $opt à "?"
        echo "ERROR - on a rencontré une erreur, mais on va poursuivre le script..."
    ;;
    esac
done

# On passe dans la suite du script !
echo "LA SUITE DU SCRIPT EST APPELÉE"

########################################################################################################################
# Fin du fichier.
########################################################################################################################

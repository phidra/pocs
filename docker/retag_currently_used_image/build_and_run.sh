#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

Cette POC ne se déroule pas autoamtiquement.
Elle contient juste un Dockerfile de support pour investiguer ce qui m'intéresse.

Objectif = montrer qu'on peut retagger sans souci une image en cours d'utilisation : le container qui l'utilise
continue de pouvoir la référencer grâce à son id.



UTILISATION TYPIQUE :

Repartir d'un état propre :
    docker system prune -a

Builder une image taggée "looper" (qui tourne en boucle) et l'exécuter :
    docker build -t looper .
        Successfully built 04e317790d9c
        Successfully tagged looper:latest
    docker run --rm looper
        # affichage de la date en boucle infinie toutes les secondes


L'image est bien en train de tourner :
    docker image list -a
        REPOSITORY   TAG       IMAGE ID       CREATED          SIZE
        looper       latest    04e317790d9c   12 seconds ago   4.86MB
        <none>       <none>    3301830dccd2   12 seconds ago   4.86MB
        busybox      latest    7cfbbec8963d   10 days ago      4.86MB

Modifier le Dockerfile, et rebuilder une AUTRE image, en déplaçant le tag :
    vim Dockerfile  # modifier des trucs
    docker build -t looper .
        Successfully built 8421eff221d5
        Successfully tagged looper:latest
    docker image list -a
        REPOSITORY   TAG       IMAGE ID       CREATED          SIZE
        <none>       <none>    f19cb701be1f   2 seconds ago    4.86MB
        looper       latest    8421eff221d5   2 seconds ago    4.86MB
        <none>       <none>    04e317790d9c   41 seconds ago   4.86MB
        <none>       <none>    3301830dccd2   41 seconds ago   4.86MB
        busybox      latest    7cfbbec8963d   10 days ago      4.86MB

À ce stade, le container qui tourne (et qui avait été lancé sur l'image looper:latest) continue de tourner sans souci,
alors même que l'image taggée looper:latest pointe sur une autre image...

En effet, le container référence bien l'image par son id :
    docker ps
        CONTAINER ID   IMAGE          COMMAND               CREATED          STATUS          PORTS     NAMES
        22a836e09fde   04e317790d9c   "/bin/ash /loop.sh"   46 seconds ago   Up 46 seconds             stupefied_johnson

EOF

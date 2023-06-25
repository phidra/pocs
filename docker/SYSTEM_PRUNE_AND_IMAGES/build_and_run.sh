#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

Cette POC n'en est pas vraiment une : c'est juste un Dockerfile de support pour investiguer le fonctionnement des images.

Exemple : je l'ai utilisée pour confirmer que les images intermédiaires n'étaient pas dangling, malgré leur absence de tag :

    # on repart d'un état vierge et on vérifie que l'état est bien vierge :
    docker system prune -a
    docker image list -a
        REPOSITORY        TAG       IMAGE ID       CREATED          SIZE

    # je builde mon image, et je vérifie qu'il n'y a pas d'images dangling :
    docker build -t poc-test-layers .
    docker system prune
        [...]
        Total reclaimed space: 0B

    # je regarde les images existantes :
    docker image list
        REPOSITORY        TAG       IMAGE ID       CREATED          SIZE
        poc-test-layers   latest    fb896daec53a   47 seconds ago   4.86MB
        busybox           latest    bab98d58e29e   6 days ago       4.86MB

    # je regarde les images intermédiaires, et j'y retrouve les images de mes commandes RUN successives :
    docker image list -a
        REPOSITORY        TAG       IMAGE ID       CREATED          SIZE
        <none>            <none>    4079343e2d38   48 seconds ago   4.86MB
        poc-test-layers   latest    fb896daec53a   48 seconds ago   4.86MB
        <none>            <none>    3610a01dab8a   49 seconds ago   4.86MB
        <none>            <none>    3cd956439997   49 seconds ago   4.86MB
        <none>            <none>    0af17a12a774   50 seconds ago   4.86MB
        <none>            <none>    c8b87437e391   50 seconds ago   4.86MB
        busybox           latest    bab98d58e29e   6 days ago       4.86MB

    # comme elles n'ont pas été supprimées par "docker system prune", alors pourtant qu'elles n'ont pas de tag,
    # j'en déduis que les images intermédiaires ne sont pas considérées comme dangling (même malgré l'absence
    # de tag) tant qu'une image parente pointe vers elle.


Autre exemple = vérifier expérimentalement que le type par défaut d'un 'docker run --mount' est 'type=bind' :

    # les messages d'erreurs sont identiques si on précise `type=volume` ou si on ne passe pas le `type` :
    docker run --rm -it --mount             source=/tmp,target=/hosttmp testlayers /bin/ash
    docker run --rm -it --mount type=volume,source=/tmp,target=/hosttmp testlayers /bin/ash

    # le message d'erreur :
    # docker: Error response from daemon: create /tmp: "/tmp" includes invalid characters for a local volume name, only "[a-zA-Z0-9][a-zA-Z0-9_.-]" are allowed. If you intended to pass a host directory, use absolute path.

    # À l'inverse, si je précise `type=bind`, ça passe :
    docker run --rm -it --mount type=bind,source=/tmp,target=/hosttmp testlayers /bin/ash

    # J'en déduis que le type par défaut de `--mount` est `volume`.

    # On peut le vérifier aussi en inspectant le container :
    docker run --rm -it --mount source=tmp,target=/hosttmp testlayers /bin/ash
    docker inspect testlayers
    # "Mounts": [
    #     {
    #         "Type": "volume",
    #         "Source": "tmp",
    #         "Target": "/hosttmp"
    #     }
    # ],

EOF

# build :
# docker build -t poc-test-layers .

# run :
# docker run --rm -it testlayers

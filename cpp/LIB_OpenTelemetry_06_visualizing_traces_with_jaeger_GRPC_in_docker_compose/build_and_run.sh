#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


cat << EOF
Cette POC démontre la visualisation de traces OTel avec jaeger en les exportant en GRPC.

Par rapport aux POCS précédentes :

- elle utilise GRPC en utilisant les dépendances requises dans un Dockerfile
- elle est lancée via un docker-compose


HOWTO utiliser la POC :
    STEP 1 = builder l'image docker (attention, la compilation d'OTel est loooooongue) :
        docker build -t poc-opentelemetry-06 .
    STEP 2 = lancer le docker-compose :
        docker-compose down ; docker-compose up
    STEP 3 = consulter les traces sur l'IHM jaeger :
        http://localhost:16686

ATTENTION : s'il y a une chose à retenir, c'est que compiler GRPC soi-même pour faire en sorte
que opentelemetry-cpp l'utilise est *compliqué* ; mieux vaut utiliser la libgrpc++-dev packagée.

EOF

#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


cat << EOF

Cette POC donne un exemple d'utilisation du SDK AWS pour télécharger un fichier dans un bucket S3.
    ATTENTION : elle ne fonctionne réellement que si les credentials AWS sont configurés
    (mais elle reste intéressante même si ça n'est pas le cas, pour la compil + utilisation du SDK)

On en profite au passage pour montrer la compilation du SDK dans le docker (qui est assez longue, d'ailleurs).
    ATTENTION : pour consulter la doc du SDK, il faut cliquer sur l'onglet "Modules" :
        https://sdk.amazonaws.com/cpp/api/LATEST/root/html/usergroup0.html
    Par exemple, la doc des classes S3 est ici :
        https://sdk.amazonaws.com/cpp/api/LATEST/aws-cpp-sdk-s3/html/annotated.html
    Si besoin, aller voir les exemples qui accompagnent le SDK :
        https://github.com/awsdocs/aws-doc-sdk-examples/tree/main/cpp
        https://github.com/awsdocs/aws-doc-sdk-examples/blob/main/cpp/example_code/s3/get_object.cpp

EOF

docker build -t poc-aws-sdk-01 . && docker run --rm -it poc-aws-sdk-01

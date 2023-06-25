#!/bin/bash

set -o errexit
set -o nounset

# only ENTRYPOINT is defined in Dockerfile, using shell form :
trap 'rm -f "$DOCKERFILE"' EXIT
DOCKERFILE=$(mktemp --suffix "_DOCKERFILE_FOR_TESTS") || exit 1
IMAGE="delete_this_test"
cat << EOF > "$DOCKERFILE"
FROM alpine:latest
ENTRYPOINT /bin/sh -c "echo salut le monde"
EOF

echo ""
echo ""
echo "Building image :"
docker build -t $IMAGE -f "$DOCKERFILE" .


echo ""
echo ""
echo "RUNNING without arguments :"
docker run --rm -it $IMAGE


echo ""
echo ""
echo "RUNNING with arguments 'echo youpi' :"  # on pourrait lancer python
docker run --rm -it $IMAGE echo youpi


echo ""
echo ""
echo "RUNNING with arguments 'this-is-an-unexisting-command' :"
docker run --rm -it $IMAGE this-is-an-unexisting-command

echo ""
echo ""
echo "En résumé : avec un ENTRYPOINT (en shell form), peu importe CMD ou les paramètres supplémentaires de 'docker run', seul ENTRYPOINT sera exécuté"

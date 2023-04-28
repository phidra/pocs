#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

POC pour jouer avec des requêtes HTTP asynchrones dans node.js, avec async/await ou des callbacks.

Pour utiliser :
    npm install
    npm run poc1
    npm run poc2
    npm run poc3
    npm run poc4


Il y a beaucoup de choses apprises, voir les notes dans chacune des POCs.

TL;DR : on peut utiliser http(s).get wrappé dans une Promise pour faire des requêtes HTTP dans node.js avec async.await.

(des versions récentes de node permettent nativement des requêtes asynchrones avec async/await via l'API fetch)
EOF

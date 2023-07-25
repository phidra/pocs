#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


echo ""
echo "POC sur le parsing de paramètres des requêtes GET par axum."
echo "La POC lance un server → le requête avec un browser (chaque page indique les URLs utiles)"
echo ""

cargo run

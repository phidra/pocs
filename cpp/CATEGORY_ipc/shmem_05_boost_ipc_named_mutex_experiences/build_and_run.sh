#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
# set -o xtrace

BUILD_DIR="NOGIT_build"
mkdir -p "${BUILD_DIR}"
cmake -B "${BUILD_DIR}" .
make -C "${BUILD_DIR}" -j

BINARY="${BUILD_DIR}/mutex_god"

"$BINARY" description ""

echo ""
MUTEX="my_super_mutex"

echo -e "\n\n=== STEP 1 = on commence par partir d'un état propre = sans aucun mutex pré-existant :"
"$BINARY" force_remove "$MUTEX"
"$BINARY" check_mutex "$MUTEX"

echo -e "\n\n=== STEP 2 = on crée le mutex, on le lock, puis on quitte le process"
set +o errexit
"$BINARY" lock_then_abort "$MUTEX"
set -o errexit

echo -e "\n\n=== STEP 3 = à ce stade, aucun process n'utilise le mutex, pourtant il existe, et il est même locké :"
"$BINARY" check_mutex "$MUTEX"

echo -e "\n\n=== STEP 4 = on ne peut plus l'unlocker proprement (car le process qui l'avait locké est mort) mais on peut forcer sa suppression :"
"$BINARY" force_remove "$MUTEX"

echo -e "\n\n===  STEP 5 = à on vérifie que le mutex n'existe plus :"
"$BINARY" check_mutex "$MUTEX"

echo -e "\n\n=== STEP 6 = un remove sur un mutex non-existant n'a pas d'impact :"
"$BINARY" force_remove "$MUTEX"
"$BINARY" check_mutex "$MUTEX"

echo ""
echo ""
echo "END OF POC"

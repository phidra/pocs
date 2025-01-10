#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

echo ""
echo "Projet automatiquement construit par cette commande :"
echo "    meson init --name pouet --build"
echo ""
echo "Derrière, je cleane + recrée le répertoire de build."
echo ""
echo ""

set -o xtrace

meson setup build

meson compile -C build

build/pouet

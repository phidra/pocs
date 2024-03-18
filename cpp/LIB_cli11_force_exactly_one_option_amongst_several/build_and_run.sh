#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null

# téléchargement de cli11 en mode single-header :
mkdir -p "$this_script_parent/CLI/"
CLI11_HPP="$this_script_parent/CLI/CLI.hpp"
VERSION="v2.4.1"  # update manually if need be
CLI11_URL="https://github.com/CLIUtils/CLI11/releases/download/${VERSION}/CLI11.hpp"
if [ ! -e "$CLI11_HPP" ]
then
    echo "Downloading CLI11 from $CLI11_URL"
    wget --output-document "$CLI11_HPP" "$CLI11_URL"
fi


# le fichier utilisé par CLI11 doit exister :
FILE="$this_script_parent/NOGIT_file.txt"
echo "dummy content" > "$FILE"


# compilation :
rm -rf NOGIT_build
CXX=clang++-14 meson setup NOGIT_build/ .
CXX=clang++-14 meson compile  -C NOGIT_build/


# une fois n'est pas coutume, la description de la POC n'est pas dans le main,
# car je vais le lancer plusieurs fois :
cat << EOF

CE QUE MONTRE CETTE POC = utilisation de CLI11

En plus de montrer le setup pour utiliser CLI11, la POC montre comment forcer l'utilisateur à préciser
exactement une option parmi plusieurs, et comment gérer l'option reçue avec un variant.

Ici, on le force à préciser une "source", qui peut être soit un fichier local, soit une URL.

Pour cela, il faut :

- définir ses options dans un option_group
- forcer le groupe à nécessiter exactement une option
- placer le parsing des deux options dans le même variant...
- ... ce qui nécessite de préciser explicitement les template-parameters de add_option<..., ...>
EOF

echo -e "\n-------"
NOGIT_build/bin  --help

# avec l'une des deux options obligatoires :
echo -e "\n-------"
NOGIT_build/bin  --from-file="$FILE"  --other-option=42

# avec l'autre des deux options obligatoires :
echo -e "\n-------"
NOGIT_build/bin  --from-url="mymethod/myhost/mypath"  --other-option=43

# si on ne passe pas aucune des deux options obligatoires, ça échoue :
set +o errexit
echo -e "\n-------"
NOGIT_build/bin  --other-option=43

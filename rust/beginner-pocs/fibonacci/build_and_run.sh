#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace


cat << EOF
POC simpliste pour jouer avec rust :
- définir une fonction
- parser un argument sur la CLI
- premiers ifs et matchs
- gérer les erreurs
- utilisation de Option et Result

EOF

# on lance la POC avec différents arguents :
cargo run -- 10
cargo run -- 11
cargo run -- 12
cargo run        # on s'attend à l'utilisation d'un arg par défaut

set +o errexit  # les deux commandes ci-dessous échouent, c'est normal, ça fait partie de la POC
cargo run -- pouet  # l'argument n'est pas un entier
cargo test       # l'un des tests échoue (volontairement, c'est pour la POC)

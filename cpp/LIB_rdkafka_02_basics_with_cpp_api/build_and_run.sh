#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null


cat << EOF


Cette POC est une simplification de cet exemple de la lib :
    https://github.com/confluentinc/librdkafka/blob/2587cac70f83fced42c51f921bed325a434f5bc7/examples/rdkafka_example.cpp

Elle démontre l'utilisation de l'API C++ de librdkafka pour envoyer/recevoir des messages sur un topic.

Quelques enseignements :

- il y a une doc Doxygen de la librairie :
    https://docs.confluent.io/platform/current/clients/librdkafka/html/index.html
- on dirait que le producer doit appeler poll régulièrement :
    https://github.com/confluentinc/librdkafka/blob/2587cac70f83fced42c51f921bed325a434f5bc7/src-cpp/rdkafkacpp.h#L1533
- conservation des messages par kafka :
    - avec cette POC, kafka garde en mémoire tous les events émis.
    - par ailleurs, le consumer est configuré pour lire tous les messages depuis le début
    - donc si kafka a des messages, à chaque fois qu'on lancera un consumer, on récupèrera tous ces "anciens" messages
    - dit autrement, ça n'est pas du pub/sub

EOF


LIBRDKAFKA_INSTALL_DIR="${this_script_parent}/NOGIT_install_librdkafka"

# VÉRIFICATION QUE LIBRDKAFKA A ÉTÉ INSTALLÉE :
if [ ! -d "$LIBRDKAFKA_INSTALL_DIR" ]
then
    cat << EOF

ERREUR : cette POC nécessite d'avoir installé manuellement librdkafka, comme suit :

# NOTE : la POC a été testée avec la branche v2.4.0 de kafka, donc je clone cette version (à bumper si besoin)
git clone --depth=1 --shallow-submodules --branch=v2.4.0 https://github.com/confluentinc/librdkafka.git NOGIT_librdkafka
cd NOGIT_librdkafka

# pour builder kafka en statique et sans dépendances :
cmake -H. -B_cmake_build \\
    -DCMAKE_INSTALL_PREFIX=$LIBRDKAFKA_INSTALL_DIR \\
    -DENABLE_LZ4_EXT=OFF \\
    -DRDKAFKA_BUILD_EXAMPLES=OFF \\
    -DRDKAFKA_BUILD_STATIC=ON \\
    -DRDKAFKA_BUILD_TESTS=OFF \\
    -DWITH_CURL=OFF \\
    -DWITH_PLUGINS=OFF \\
    -DWITH_SASL=OFF \\
    -DWITH_SSL=OFF \\
    -DWITH_ZLIB=OFF \\
    -DWITH_ZSTD=OFF

cmake --build _cmake_build
cmake --build _cmake_build --target install
    vérifier dans les logs qu'on a bien installé librdkafka au bon endroit :
    -- Installing: (...)
    -- Installing: /path/to/poc/NOGIT_install_librdkafka/lib/librdkafka++.a
    -- Installing: /path/to/poc/NOGIT_install_librdkafka/include/librdkafka/rdkafkacpp.h
    -- Installing: (...)
cd ..

EOF
    exit 1
fi


# COMPILATION :
echo "Compilation du producer et du consumer :"
rm -rf NOGIT_build
 CXX=clang++-14 meson setup NOGIT_build/ .
 CXX=clang++-14 meson compile  -C NOGIT_build/


# INSTRUCTIONS :
cat << EOF
...les binaires ont maintenant été compilés, reste à lancer la POC manuellement, dans plusieurs terminaux :

STEP 0 = dans un premier terminal, lancer :
    (si besoin : docker container prune --force)
    docker-compose up

STEP 1 = dans un second terminal, produire les messages :
    NOGIT_build/bin_producer TARTIFLETTE  msg1  msg2  msg99

STEP 2 = consommer les messages :
    NOGIT_build/bin_consumer TARTIFLETTE

EOF

#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null


cat << EOF

Cette POC illustre la possibilité de faire communiquer un consumer en C++ et un producer python.

EOF



# LIBRDKAFKA :
################################################################################
LIBRDKAFKA_INSTALL_DIR="${this_script_parent}/NOGIT_install_librdkafka"
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

# VENV PYTHON :
################################################################################
venv=NOGIT_mysupervenv
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import confluent_kafka' &> /dev/null
then
    "$venv"/bin/pip install -U confluent-kafka
fi

# COMPILATION :
################################################################################
echo "Compilation du producer et du consumer :"
rm -rf NOGIT_build
 CXX=clang++-14 meson setup NOGIT_build/ .
 CXX=clang++-14 meson compile  -C NOGIT_build/


# INSTRUCTIONS :
################################################################################
cat << EOF
...les binaires ont maintenant été compilés, reste à lancer la POC manuellement, dans plusieurs terminaux :

STEP 0 = dans un premier terminal, lancer :
    (si besoin : docker container prune --force)
    docker-compose up

STEP 1 = dans un autre terminal, lancer un consumer :
    NOGIT_build/bin_consumer TARTIFLETTE

STEP 2 = dans un troisième terminal, lancer le producer python :
    $venv/bin/python producer.py


-----------> constater que le consumer C++ reçoit bien le message envoyé depuis le producer PYTHON
EOF

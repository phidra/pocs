#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null


cat << EOF


Cette POC est la suite de la précédente, elle démontre le pubsub avec l'API C++ de kafka.

Ce qui change par rapport à la précédente = on se rapproche d'un pubsub classique :

- les consumers ne consomment plus TOUS les messages du broker, mais uniquement ceux après leur démarrage
- on montre qu'en lançant plusieurs consumers, chacun reçoit les messages du broker

Idéalement, j'aurais aussi souhaité que le borker ne garde pas trace des messages (i.e. ils sont forwardés
aux consumers puis oubliés aussitôt). En pratique, il existe une "retention policy" pour cela :
    https://ondrej-kvasnovsky.medium.com/how-to-delete-messages-from-kafka-22c1e1de3600
    ^ on peut au moins utiliser kafka-config.sh pour ça :
        ./bin/kafka-configs.sh \
              --alter \
              --entity-type topics \
              --entity-name my-topic \
              --add-config retention.ms=1000
    https://docs.confluent.io/platform/current/installation/configuration/topic-configs.html#cleanup-policy
    ^ on peut choisir quoi faire des messages, la policy par défaut est de delete les vieux messages
    (l'alternative est de compacter les logs après un certain temps, et on peut même faire les deux : compact puis delete)
    https://docs.confluent.io/platform/current/installation/configuration/topic-configs.html#retention-ms
    ^ par défaut, les messages sont supprimés après 7 jours
En pratique, j'ai eu beau joué avec la configuration, je n'ai pas réussi à setter "retention.ms".
Comme je n'ai pas 3 jours à y passer + que c'est surtout un sujet admin de toutes façons, j'abandonne ce point.

J'ajoute tout de même à la POC un binaire permettant de dumper la configuration.
(si je réussis plus tard, je reviendrais amender la poc pour setter une durée de rétention d'une seconde)


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

STEP 1 = dans plusieurs autres terminaux, lancer les consumers :
    NOGIT_build/bin_consumer TARTIFLETTE
    NOGIT_build/bin_consumer TARTIFLETTE
    (...)

STEP 2 = dans un autre terminal, produire les messages :
    NOGIT_build/bin_producer TARTIFLETTE  msg1  msg2  msg99


----> On constate que tous les consumers reçoivent les messages émis.
    (EDIT : pas si clair, en fait... on dirait que certains messages peuvent être zappés par un consumer ?!)

STEP 3 = lancer un nouveau consumer, pour constater qu'il ne consomme pas les anciens messages :
    NOGIT_build/bin_consumer TARTIFLETTE


STEP 4 = lancer le message-dumper pour constater que les anciens messages sont pourtant bien sur le broker :
    NOGIT_build/bin_dump_all_messages TARTIFLETTE


side-note : pour dumper la config :
    NOGIT_build/bin_showconf

EOF

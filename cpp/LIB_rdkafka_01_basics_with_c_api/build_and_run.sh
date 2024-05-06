#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null


echo ""
echo ""
echo "Cette POC est tirée du getting-started de kafka :"
echo "    https://developer.confluent.io/get-started/c/"
echo ""
echo "Elle démontre l'utilisation de librdkafka pour envoyer/recevoir des messages sur un topic"
echo ""


LIBRDKAFKA_INSTALL_DIR="${this_script_parent}/NOGIT_install_librdkafka"

# vérification que librdkafka a été installée :
if [ ! -d "$LIBRDKAFKA_INSTALL_DIR" ]
then
    cat << EOF

ERREUR : cette POC nécessite d'avoir installé manuellement librdkafka, comme suit :

git clone https://github.com/confluentinc/librdkafka.git NOGIT_librdkafka
cd NOGIT_librdkafka
cmake -H. -B_cmake_build
ccmake _cmake_build
    modifier CMAKE_INSTALL_PREFIX pour indiquer ce chemin : $LIBRDKAFKA_INSTALL_DIR
    (i.e. indiquer /path/to/poc/NOGIT_install_librdkafka)
    puis, choisir "configure" puis "quit"
cmake --build _cmake_build
cmake --build _cmake_build --target install
    vérifier dans les logs qu'on a bien installé librdkafka au bon endroit :
    -- Installing: (...)
    -- Installing: /path/to/poc/NOGIT_install_librdkafka/lib/librdkafka++.so
    -- Installing: /path/to/poc/NOGIT_install_librdkafka/include/librdkafka/rdkafkacpp.h
    -- Installing: (...)
cd ..

EOF
    exit 1
fi


# vérification de présence de la glib :
if ! pkg-config --libs glib-2.0 &> /dev/null
then
    cat << EOF
    ERREUR : cette POC nécessite d'avoir la glib installée. Si besoin :

        sudo apt install libglib2.0-dev

EOF
    exit 1
fi




echo "Compilation des binaires..."

# compilation du binaire chargé de créer le topic (qui utilise l'API C++)
g++ src/main_create_topic.cpp -I NOGIT_install_librdkafka/include/ -L NOGIT_install_librdkafka/lib -lrdkafka -lrdkafka++ -o NOGIT_create_topic

# compilation du producer et sender :
# shellcheck disable=SC2046
gcc src/main_producer.c -Wall $(pkg-config --cflags glib-2.0) -I NOGIT_install_librdkafka/include/ -lglib-2.0 -L NOGIT_install_librdkafka/lib -lrdkafka -o NOGIT_producer
# shellcheck disable=SC2046
gcc src/main_consumer.c -Wall $(pkg-config --cflags glib-2.0) -I NOGIT_install_librdkafka/include/ -lglib-2.0 -L NOGIT_install_librdkafka/lib -lrdkafka -o NOGIT_consumer


cat << EOF
...les binaires ont maintenant été compilés, reste à lancer la POC manuellement, dans plusieurs terminaux :

STEP 0 = dans un premier terminal, lancer :
    docker-compose up

STEP 1 = dans un second terminal, créer le topic :
    LD_LIBRARY_PATH=NOGIT_install_librdkafka/lib ./NOGIT_create_topic TARTIFLETTE
    (attention, même quand le docker-compose kafka n'est pas lancé, on reçoit un log de succès trompeur)

STEP 2 = produire les messages :
    LD_LIBRARY_PATH=NOGIT_install_librdkafka/lib ./NOGIT_producer getting-started.ini

STEP 3 = consommer les messages :
    LD_LIBRARY_PATH=NOGIT_install_librdkafka/lib ./NOGIT_consumer getting-started.ini

EOF

#!/bin/bash

# Installation de perf :
#    sudo apt install linux-tools-common
#    perf --help  # permet de voir quel kernel-specific version doit être installé
#    sudo apt install linux-tools-4.15.0-62-generic linux-cloud-tools-4.15.0-62-generic
#
# download de flamegraph.pl et stackcollapse-perf.pl :
#    https://github.com/brendangregg/FlameGraph
#    wget "https://raw.githubusercontent.com/brendangregg/FlameGraph/master/stackcollapse-perf.pl"
#    wget "https://raw.githubusercontent.com/brendangregg/FlameGraph/master/flamegraph.pl"
#
# pouvoir lancer perf sans sudo :
#     sudo sysctl -w kernel.perf_event_paranoid=1"


# TODO : séparer le build des cas-tests et la POC sur perf

# AUTRES POCS À FAIRE :
# OTHER = une seule fonction lente car elle appelle de l'IO (requête HTTP ?) -> je dois voir quoi ?
# OTHER = lenteur car appel de sous-process -> il se passe quoi ?
# OTHER = et perfer un script bash ? et python ?
# OTHER = et perfer un script bash/python qui appelle un sous-process ?


set -o errexit

[ "$1" == "" -o "$1" == "-h" -o "$1" == "--help" ] && echo """
USAGE : $0 case

EXAMPLE : $0 single_heavy_function.cpp

AVAILABLE CASES :
$(ls *.cpp)
""" && exit

set -o nounset

[ ! -e "$1" ] && echo "ERROR : inexisting file $1" && exit

echo "=== Building bin from : $1"
clang++ -std=c++11 "$1" -o bin
chmod +x bin


echo ""
echo "=== If necessary, run : "
echo "     sudo sysctl -w kernel.perf_event_paranoid=1"

echo ""
echo "=== Running perf tool on binary"
echo ""
perf record -F 199 -g ./bin

echo ""
echo "=== End of execution"


echo ""
echo "=== perf.data has been written, now building flamegraph"
perf script | ./stackcollapse-perf.pl | ./flamegraph.pl > perf.svg

echo ""
echo "=== perf.svg has been generated, now run :"
echo "    firefox perf.svg"

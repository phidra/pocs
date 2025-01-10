#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

OUTPUT_DIR="${this_script_parent}/NOGIT_output_dir"
mkdir -p "${OUTPUT_DIR}"

cat << EOF

==========
Ce que fait cette POC = affichage des bridges

INPUT = graphe osm.pbf
INPUT = polygone de filtrage (optionnel)
OUTPUT = dump geojson des bridges (et de plein d'autres données intermédiaires).

Pour un peu de théorie, cf. :
    https://en.wikipedia.org/wiki/Bridge_(graph_theory)
    https://en.wikipedia.org/wiki/Biconnected_component
    https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/biconnected_components.html

Création d'un graphe à partir des données OSM (filtrées par le polygone).
Calcul des composantes biconnexes (et dump de leur hull), et des articulation points qui vont avec.
(les composantes connexes sont calculées et dumpées aussi).
Calcul des bridges = les composantes biconnexes qui n'ont que deux vertex.
Filtrage des bridges = on dégage les bridges qui sont des cul-de-sac.
Merge des bridges adjacents = on considère N bridges adjacents comme un seul grand bridge constitué de N tronçons.
Dump en geojson de tout ce beau monde.

NOTE : attention, des bridges "artificiels" peuvent être créés à la frontière du graphe d'entrée, parce qu'il est "coupé" par le polygone.
==========

EOF

set -o xtrace
src/_build/bin/connectedcomponents \
    DATA/NOGIT_monaco.osm.pbf \
    DATA/monaco_polygon.geojson \
    "${OUTPUT_DIR}"

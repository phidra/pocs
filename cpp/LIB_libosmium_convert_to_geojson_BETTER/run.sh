#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

OUTPUT_DIR="${this_script_parent}/NOGIT_output_dir"
mkdir -p "${OUTPUT_DIR}"

set -o xtrace
src/_build/bin/osm2boostgraph \
    DATA/NOGIT_monaco.osm.pbf \
    DATA/monaco_polygon.geojson \
    "${OUTPUT_DIR}"

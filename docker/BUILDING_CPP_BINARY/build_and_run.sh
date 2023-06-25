#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

IMAGE="deleteme_poc_docker_building_cpp_binary"

docker build -t "$IMAGE" .
docker \
    run \
    --rm \
    -i \
    -t \
    -v "${this_script_parent}:/tmp/mounted" \
    --expose=9999 \
    -p 9999:9999 \
    "$IMAGE" \
    /bin/bash /tmp/mounted/script_to_run_inside_container.sh

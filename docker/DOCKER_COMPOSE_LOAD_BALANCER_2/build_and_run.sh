#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

# temporary files :
SERVED_DIR="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_load_balancer_1.XXX")"
echo "<h1> Common served file </h1>" > "${SERVED_DIR}/index.html"
chmod -R a+rx "$SERVED_DIR"

DOCKER_COMPOSE_ENVFILE="${this_script_parent}/NOGIT_environment_file"
echo "SERVED_DIR=${SERVED_DIR}" > "$DOCKER_COMPOSE_ENVFILE"


cat << EOF
This POC shows a basic usage of load-balancing with docker-compose :
- 5 http servers are launched (they all serve the same file)
- 1 load-balancer is excplicitly configured to request the servers (round-robin)

NOTE 1 : even if 5 http servers are launched, there is only ONE service definition in docker-compose file.
(but 5 instances of this service will be launched)

NOTE 2 : as all servers do serve the same file, you have to look at docker-compose logs
to ensure that all the http servers are requested, following round-robin.

EOF

echo ""
echo "DOCKER-COMPOSE CONFIG :"
docker-compose -f docker-compose.yml --env-file "$DOCKER_COMPOSE_ENVFILE" config
echo ""



cat << EOF
DOCKER-COMPOSE UP :
When docker-compose will be up, you can request the load-balancer using :
    http://localhost:7777

If you request it several times, you should see in docker-compose logs that it is not always
the same httpd container that serves the requests.

Then you can shutdown docker-compose with Ctrl+C, and remove containers with :
    docker-compose -f docker-compose.yml rm -f

Now running docker-compose up...
EOF

docker-compose \
    -f docker-compose.yml \
    --env-file "$DOCKER_COMPOSE_ENVFILE" \
    up \
    --scale mysuperserver=5  # this will run 5 instances of the mysuperserver service

echo ""
echo ""
echo "Now about to remove containers."
echo "If needed (i.e. if following command fails), you can also manually remove containers with :"
echo "    docker-compose -f docker-compose.yml rm -f"
echo ""

docker-compose -f docker-compose.yml rm -f

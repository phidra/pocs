#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

# temporary files :
SERVED_DIR1="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_basics_1.XXX")"
SERVED_DIR2="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_basics_2.XXX")"
SERVED_DIR3="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_basics_3.XXX")"
echo "<h1> Server 1 </h1>" > "${SERVED_DIR1}/index.html"
echo "<h1> Server 2 </h1>" > "${SERVED_DIR2}/index.html"
echo "<h1> Server 3 </h1>" > "${SERVED_DIR3}/index.html"
chmod -R a+rx "$SERVED_DIR1"
chmod -R a+rx "$SERVED_DIR2"
chmod -R a+rx "$SERVED_DIR3"

DOCKER_COMPOSE_ENVFILE="${this_script_parent}/NOGIT_environment_file"

cat << EOF > "$DOCKER_COMPOSE_ENVFILE"
SERVED_DIR1=${SERVED_DIR1}
SERVED_DIR2=${SERVED_DIR2}
SERVED_DIR3=${SERVED_DIR3}
EOF


echo ""
echo "This basic POC just shows how to run 3 httpd (each serving a different directory) with docker-compose."
echo ""


echo ""
echo "DOCKER-COMPOSE CONFIG :"
docker-compose -f docker-compose.yml --env-file "$DOCKER_COMPOSE_ENVFILE" config
echo ""



cat << EOF
DOCKER-COMPOSE UP :
When docker-compose will be up, you can request one of the URLs :
    http://localhost:8881
    http://localhost:8882
    http://localhost:8883

Then you can shutdown docker-compose with Ctrl+C, and remove containers with :
    docker-compose -f docker-compose.yml rm -f

Now running docker-compose up...
EOF

docker-compose -f docker-compose.yml --env-file "$DOCKER_COMPOSE_ENVFILE" up

echo ""
echo ""
echo "Now about to remove containers."
echo "If needed (i.e. if following command fails), you can also manually remove containers with :"
echo "    docker-compose -f docker-compose.yml rm -f"
echo ""

docker-compose -f docker-compose.yml rm -f

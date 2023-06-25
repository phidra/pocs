#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

# temporary files :
SERVED_DIR1="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_load_balancer_1.XXX")"
SERVED_DIR2="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_load_balancer_2.XXX")"
SERVED_DIR3="$(mktemp --tmpdir=/tmp -d "POC_docker_compose_load_balancer_3.XXX")"
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


cat << EOF
This POC shows a basic usage of load-balancing with docker-compose :
- 3 https services are run (each serving a different file, to help identify the servers)
- 1 load-balancer is excplicitly configured to request the servers (round-robin)

NOTE : all 3 https services are EXPLICITLY defined in the docker-compose file
(this is different from defining ONE single service, and using the --scale option)

EOF

echo ""
echo "DOCKER-COMPOSE CONFIG :"
docker-compose -f docker-compose.yml --env-file "$DOCKER_COMPOSE_ENVFILE" config
echo ""



cat << EOF
DOCKER-COMPOSE UP :
When docker-compose will be up, you can request the load-balancer using :
    http://localhost:7777

If you request it several times, you should see that the repsponding server changes.

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

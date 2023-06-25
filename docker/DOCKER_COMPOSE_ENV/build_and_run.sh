#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

cat << EOF

This POC shows :

- that the command 'docker-compose config' helps to debug environment variables
- that one can specify an env file using '--env-file'

NOTE : docker-compose uses a hidden '.env' file by default, that we override here with '--env-file'.

EOF

echo ""
echo "WITH ENV1, docker-compose config is resolved to  :"
docker-compose -f docker-compose.yml --env-file ENV1 config
echo ""

echo ""
echo "WITH ENV2, docker-compose config is resolved to  :"
docker-compose -f docker-compose.yml --env-file ENV2 config
echo ""

echo ""
echo "In both cases, note the 'port' lines"

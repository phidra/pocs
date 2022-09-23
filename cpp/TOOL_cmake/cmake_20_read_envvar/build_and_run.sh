#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

# from a fresh state :
unset YUM

echo ""
echo "=== STEP 1 = no envvar 'YUM' defined :"
cmake -P read_envvar.cmake

echo ""
echo "=== STEP 2 = with an envvar 'YUM' defined to 'coucou' :"
YUM=coucou cmake -P read_envvar.cmake


echo ""
echo "=== STEP 3 = with an envvar 'YUM' defined to an empty string :"
YUM='' cmake -P read_envvar.cmake

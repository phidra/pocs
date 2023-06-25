#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


echo ""
echo "This scripts runs a server on desired port, and serves data of a particular directory."
echo ""
echo "USAGE:      $0  (<port>)"
echo "EXAMPLE:    $0  8000"
echo "EXAMPLE:    $0  8001"
echo ""


this_script_parent="$(realpath "$(dirname "$0")" )"
SERVED_DIR="${this_script_parent}/content-to-serve"

# ARG = server port :
DEFAULT_PORT=44444
PORT="${1:-$DEFAULT_PORT}"
echo "Serving on port   = $PORT"
echo "Serving directory = $SERVED_DIR"
echo "You can request the server with :"
echo "    curl http://localhost:${PORT}"
echo "    curl http://localhost:${PORT}/data.json"
echo ""

python3 -m http.server --directory "$SERVED_DIR" "$PORT"


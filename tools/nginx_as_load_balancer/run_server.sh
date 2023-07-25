#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


echo ""
echo "This scripts runs a server on desired port, and serves data that identifies the server."
echo ""
echo "USAGE:      $0  (<port>)"
echo "EXAMPLE:    $0  8000"
echo "EXAMPLE:    $0  8001"
echo ""


this_script_parent="$(realpath "$(dirname "$0")" )"

# ARG = server port :
DEFAULT_PORT=44444
PORT="${1:-$DEFAULT_PORT}"

SERVED_DIR="$(mktemp --tmpdir=/tmp -d "POC_nginx_as_load_balancer_server_${PORT}.XXX")"
SERVED_FILE="${SERVED_DIR}/index.html"
# each server serves its own file, that gives its port :
cat << EOF > "$SERVED_FILE"

<!DOCTYPE html>
<html>
    <head>
        <!-- if we don't disable favicon like that, additional requests will mess up our tests... -->
        <link rel="icon" href="data:;base64,=">
    </head>
    <body>
    <h1>Response</h1>
    <p>
        This response was generated by a server listening on port $PORT
    </p>
    </body>
</html>
EOF

echo "Serving on port   = $PORT"
echo "Serving directory = $SERVED_DIR"
echo "You can request the server with :"
echo "    curl http://localhost:${PORT}"
echo "    curl http://localhost:${PORT}/index.html"
echo ""

python3 -m http.server --directory "$SERVED_DIR" "$PORT"


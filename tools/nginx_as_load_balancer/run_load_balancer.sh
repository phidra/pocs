#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


echo ""
echo "This scripts runs nginx on the port configured in conf file."
echo ""
echo "USAGE:      $0"
echo ""

this_script_parent="$(realpath "$(dirname "$0")" )"
NGINX_CONF="${this_script_parent}/nginx_load_balancer.conf"

# ARG (mandatory thanks to nounset) = server port :
PORT="$(grep listen "$NGINX_CONF" | awk -F " " '{print $2}' | cut -d ";" -f 1)"
echo "Serving on port : $PORT"
echo ""
echo "You can now request the server with :"
echo "    curl http://localhost:${PORT}/"
echo "    curl http://localhost:${PORT}/data.json"
echo ""
echo "NOTE : an alert with this message is 'normal' :"
echo '    nginx: [alert] could not open error log file: open() "/var/log/nginx/error.log" failed (13: Permission denied)'
echo "cf. https://stackoverflow.com/questions/34258894/nginx-still-try-to-open-default-error-log-file-even-though-i-set-nginx-config-fi/65330408#65330408"
echo "====================================================="
echo ""

nginx -c "$NGINX_CONF" -p "$this_script_parent"

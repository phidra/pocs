#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC illustrate zmq REQ-REP
(it requires pyzmq)

"""

echo ""
echo "Running REPLYER in background :"
echo "If necessary, command to kill it :"
echo ""
echo "    pkill -f -n  python3 ${this_script_parent}/reply.py"
echo ""
python3 "${this_script_parent}/reply.py" &

sleep 1
echo ""
echo "Running requester :"
python3 "${this_script_parent}/request.py"
echo ""

# cleaning :
echo ""
echo "Killing background replyer"
pkill -n -f "python3 ${this_script_parent}/reply.py"
echo "Done"
echo ""
echo ""

echo "If there is still a reply.py process below, you'll have to kill it manually :"
pgrep -f "python3 ${this_script_parent}/reply.py"

#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC shows basic usage of zmq pub/sub sockets : a publisher sends messages to a subscriber.
(it requires pyzmq)

"""

echo ""
echo "Running susbcriber in background :"
echo "If necessary, command to kill it :"
echo ""
echo "    pkill -f -n  python3 ${this_script_parent}/subscribe.py"
echo ""
python3 "${this_script_parent}/subscribe.py" &

sleep 1
echo ""
echo "Running publisher :"
python3 "${this_script_parent}/publish.py" pouet coucou youpi
echo ""

# cleaning :
echo ""
echo "Killing background subscriber"
pkill -n -f "python3 ${this_script_parent}/subscribe.py"
echo "Done"
echo ""
echo ""

echo "If there is still a subscribe.py process below, you'll have to kill it manually :"
pgrep -f "python3 ${this_script_parent}/subscribe.py"

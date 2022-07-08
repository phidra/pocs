#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC illustrate zmq slow-joiner.
(it requires pyzmq)

what is happening :
- a subscriber is launched first (and we wait sufficiently for it to be properly started)
- a publisher is then launched, and as soon as it is launched, it calls several SEND
- these first published messages are LOST (event if subscriber was launched !)
- to avoid that, an easy solution is for publisher to sleep before publishing, to wait for connection to be live
- the message sent after the sleep is the only one to be properly received by subscriber

TL;DR : beware, the first published messages might be lost (hacky solution = sleep before sending)

(See "slow joiner" in ZMQ guide : https://zguide.zeromq.org/docs/chapter1/ )
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
python3 "${this_script_parent}/publish.py"
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

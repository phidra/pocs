#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC demonstrates how to subscribe to an interesting message without having to prefix it by the topic.

Here, we want to send a json dict, but in order for the subscriber to receive it, it must be prefixed by the topic...
How to do this ?

TL;DR = send the couple {topic+interesting_message} as multipart :
    - sending topic first ensures the subscriber receives the couple
    - using a multipart message ensures the subscriber can then focus on the second part without having to unprefix it

"""

echo ""
echo "Running subscriber script in background :"
echo "If necessary, command to kill it :"
echo ""
echo "    pkill -f -n  python3 ${this_script_parent}/subscribe.py"
echo ""
python3 "${this_script_parent}/subscribe.py" &

sleep 1
echo ""
echo "Publishing messages :"
python3 "${this_script_parent}/publish.py"


# cleaning :
echo ""
echo "Killing background subscriber"
pkill -n -f "python3 ${this_script_parent}/subscribe.py"
echo "Done"
echo ""
echo ""

echo "If there is still a subscribe.py process below, you'll have to kill it manually :"
pgrep -f "python3 ${this_script_parent}/subscribe.py"

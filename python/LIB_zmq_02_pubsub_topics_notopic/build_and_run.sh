#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC investigates the usage of topics in pub/sub sockets (it requires pyzmq)

What we learn :

- there is nothing special with topics, topics are just the first few bytes of the messages
- nominal  = if a subscriber defines a topic, it receives only the messages STARTING by this topic
- wildcard = if a subscriber defines an EMPTY topic, it receives ALL the messages
- multiple = a subscriber can define several topics, it receives all the MATCHING messages
- bugged   = if a subscriber does NOT define any topic, it does NOT receive any message

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
echo "Publishing several messages :"


echo "=== Messages with topic A (will be receive by : empty_topic / topic_A / both_A_and_B) :"
python3 "${this_script_parent}/publish_a_message.py" "A message starting with topic A"
python3 "${this_script_parent}/publish_a_message.py" "Aidem without space"
sleep 0.2
echo ""

echo "=== Messages with topic B (will be receive by : empty_topic / topic_B / both_A_and_B) :"
python3 "${this_script_parent}/publish_a_message.py" "B message starting with topic B"
python3 "${this_script_parent}/publish_a_message.py" "Bidem without space"
sleep 0.2
echo ""

echo "=== Message with topic C (will be receive by : empty_topic) :"
# message with topic C (subscribers that will receive it = empty_topic)
python3 "${this_script_parent}/publish_a_message.py" "C message without matching topic"
sleep 0.2
echo ""

echo "=== NOTE : subscriber 'no_topic' will never receive any message"


# cleaning :
echo ""
echo "Killing background subscriber"
pkill -n -f "python3 ${this_script_parent}/subscribe.py"
echo "Done"
echo ""
echo ""

echo "If there is still a subscribe.py process below, you'll have to kill it manually :"
pgrep -f "python3 ${this_script_parent}/subscribe.py"

#!/bin/bash

set -o errexit
set -o nounset


this_script_parent="$(realpath "$(dirname "$0")" )"

echo """
This POC investigates multipart messages (it requires pyzmq)

what is happening :
- PUBLISHER sends a lot of messages with SNDMORE, then waits before sending a single message without SNDMORE
- SUBSCRIBER does not receive anything until the no-SNDMORE is actually sent

why :
- publisher sends several messages, receiver recv several times BUT on-wire, only one batch of messages is actually sent
- the multiple messages are not really sent on the socket until a message without SNDMORE is sent
- so multipart acts kinda like a transaction : either all messages are received or none

notes :
- kinda counter-intuitive note : you have to use one recv for each send (even if messages are sent together, you do NOT use a single RECV for the whole multipart message)
- for receiver it's transparent, it uses recv normally
- for sender, he must set the SNDMORE flag on all messages but the last
- the "transaction" will be sent with the first no-SNDMORE call to send

https://zguide.zeromq.org/docs/chapter2/#Working-with-Messages
- ZeroMQ also supports multipart messages, which let you send or receive a list of frames as a single on-the-wire message.
- multipart messages [are] quite simply series of frames with a “more” bit set to one, followed by one with that bit set to zero.
- ZeroMQ does not send the message (single or multipart) right away, but at some indeterminate later time. A multipart message must therefore fit in memory.
- Using multipart data will not reduce memory consumption.

https://zguide.zeromq.org/docs/chapter2/#Multipart-Messages
- When you send a multipart message, the first part (and all following parts) are only actually sent on the wire when you send the final part.
- If you are using zmq_poll(), when you receive the first part of a message, all the rest has also arrived.
- You will receive all parts of a message, or none at all.
- You will receive all parts of a message whether or not you check the more property.
- There is no way to cancel a partially sent message, except by closing the socket.

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

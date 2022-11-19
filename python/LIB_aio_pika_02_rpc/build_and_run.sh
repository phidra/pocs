#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


myvenv=NOGIT_venv
[ ! -d "$myvenv" ] && /usr/bin/env python3 -m venv  "$myvenv"

if ! "$myvenv"/bin/python -c 'import aio_pika ; import asyncio' &> /dev/null
then
    "$myvenv"/bin/pip install -U pip wheel
    "$myvenv"/bin/pip install aio_pika
    "$myvenv"/bin/pip install asyncio
fi

echo ""
echo "=== RUNNING"
# first receiver in background (Ctrl+C needed to kill it) :
background_jobs=()  # this array stores the pids of the background jobs
"$myvenv"/bin/python "${this_script_parent}/rpc_receive.py" &
background_jobs+=($!)

# then sender, multiple times :
"$myvenv"/bin/python "${this_script_parent}/rpc_send.py" 25
"$myvenv"/bin/python "${this_script_parent}/rpc_send.py" 28
"$myvenv"/bin/python "${this_script_parent}/rpc_send.py" 30


# killing background job :
for pid in "${background_jobs[@]}"; do
  echo "Killing background job = $pid"
  kill "$pid"
  echo "Done, waiting..."
  wait "$pid"
  echo "Ok"
done

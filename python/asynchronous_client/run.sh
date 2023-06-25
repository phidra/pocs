#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

DESCRIPTION = implementation of an asynchronous AND multiprocessed client to request URLs.

URLs are read from a requestfile by a feeder process and put in a queue.
(the URLs were gotten from : https://fakerapi.it/en)
(if need be, there are other test APIs here :  https://github.com/public-apis/public-apis#test-data)

Some coroutines consume the URLs in the queue, and request them.

Later, results are aggregated and dumped in a single json.

There are 2 kinds of concurrency used in this client :
- number of consumer processes to consume the queue and request the URLs
- number of coroutines spawned by process

EOF

this_script_parent="$(realpath "$(dirname "$0")" )"


venv="${this_script_parent}/NOGIT_venv_poc"
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"

if ! "$venv"/bin/python -c 'import aiohttp' &> /dev/null
then
    "$venv"/bin/pip install -U pip wheel
    "$venv"/bin/pip install -r "${this_script_parent}/requirements.txt"
fi


"$venv"/bin/python asynchronous-client/main.py \
    --num-client-processes 2 \
    --num-client-coro 4 \
    "${this_script_parent}/INPUT_urls.txt" \
    --output="${this_script_parent}/NOGIT_results.json"

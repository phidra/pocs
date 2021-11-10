#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

cat << EOF
The goal of this POC is to run concurrent jobs.
The jobs illustrated here are dumb (they display an argument then sleep)

The POC is splitted in 2 parts : without error management, then with.

Without error management, on my 4-cores-CPU :
    running the jobs sequentially need ~4.5 seconds
    running the jobs concurrently need ~2 seconds (the duration of the longest job)

NOTE : don't forget to use 'wait' to prevent the main script to return too early.


With error management, the goal is for the main script to fail if one of the background jobs fails.
    sequential : it's quite easy, using 'trap ERR'
    concurrent : it's more complicated, involving individual 'wait' + one global wait, see the POC, and :
		https://gist.github.com/mjambon/79adfc5cf6b11252e78b75df50793f24

As the concurrent error management is quite complicated, it may not always work, e.g. may cause deadlocks,
or fail to handle the situation where a job has already returned when we call its individual 'wait'

A further refinement (not tested in this POC) would be to limit the number of jobs, e.g. to the number of cores :
    https://stackoverflow.com/questions/38160/parallelize-bash-script-with-maximum-number-of-processes/881392#881392
    https://medium.com/@robert.i.sandor/getting-started-with-parallelization-in-bash-e114f4353691
EOF

echo ""
echo "RUNNING THE SEQUENTIAL SCRIPT :"
/usr/bin/time -p "${this_script_parent}/sequential.sh"

echo ""
echo "RUNNING THE CONCURRENT SCRIPT :"
/usr/bin/time -p "${this_script_parent}/concurrent.sh"

set +o errexit  # we expect the next scripts to fail

echo ""
echo "RUNNING THE SEQUENTIAL SCRIPT, WITH ERRORS :"
/usr/bin/time -p "${this_script_parent}/with_error_sequential.sh"

echo ""
echo "RUNNING THE CONCURRENT SCRIPT, WITH ERRORS :"
/usr/bin/time -p "${this_script_parent}/with_error_concurrent.sh"

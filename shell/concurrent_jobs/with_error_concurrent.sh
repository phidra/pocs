#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
# shellcheck source=shell/concurrent_jobs/lib.sh
source "${this_script_parent}/lib.sh"

trap 'on_error' ERR

on_error() {
  echo ""
  echo "KO ! An error has occurred..."
  echo ""
  # we trapped on the first individual 'wait' that failed
  # but we still want to wait for all the other jobs to finish before exiting
  # thus, we also wait here :
  wait

  exit 1
}

background_jobs=()  # this array stores the pids of the background jobs
for person in "${force_wielders[@]}"
do
    # when a background job is started, we store its pid
    if [ "$person" == "anakin" ]
    then
        sleep_and_print_failing_on_yoda "$person" 2 &
        background_jobs+=($!)
    else
        sleep_and_print_failing_on_yoda "$person" &
        background_jobs+=($!)
    fi
done

# we want for 'wait' to return an error when a background job failed
# cf. man bash 'the return status is the exit status of the last process or job waited for'
# thus, waiting for each job individually allows for 'wait' to return an error when a job failed
# EDIT : this is actually far from ideal, as we wait for jobs in order.
# consider this situation :
#   the first job will be successfull but very long to execute
#   the second job will fail as soon as started
# then, as we will 'wait' for the first job, we will detect the second job failure only AFTER the first job has returned...
# this could probably be better handled by 'wait -n' but it doesn't really seem to work with zsh...
for pid in "${background_jobs[@]}"; do
  wait "$pid"  # this will trig the trap if a background job failed
done

# and we (rightfully !) never go here, because we successfully trapped the error :-)
echo "If we get here, we didn't trap any error... (which is actually not desired)"

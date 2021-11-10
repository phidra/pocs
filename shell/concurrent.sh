#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
# shellcheck source=shell/lib.sh
source "${this_script_parent}/lib.sh"

for person in "${force_wielders[@]}"
do
    if [ "$person" == "anakin" ]
    then
        sleep_and_print "$person" 2 &
    else
        sleep_and_print "$person" &
    fi
done

# This 'wait' is important in order for the main script to wait for the background jobs
wait

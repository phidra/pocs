#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
# shellcheck source=shell/concurrent_jobs/lib.sh
source "${this_script_parent}/lib.sh"

for person in "${force_wielders[@]}"
do
    if [ "$person" == "anakin" ]
    then
        sleep_and_print "$person" 2
    else
        sleep_and_print "$person"
    fi
done

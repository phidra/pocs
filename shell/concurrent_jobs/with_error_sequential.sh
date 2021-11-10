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
  exit 1
}

for person in "${force_wielders[@]}"
do
    if [ "$person" == "anakin" ]
    then
        sleep_and_print_failing_on_yoda "$person" 2
    else
        sleep_and_print_failing_on_yoda "$person"
    fi
done

echo "If we get here, we didn't trap any error... (which is actually not desired)"

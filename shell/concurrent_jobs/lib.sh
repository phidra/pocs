#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

function sleep_and_print() {
    local id="$1"
    local seconds_to_sleep=${2:-0.5}
    sleep "$seconds_to_sleep"
    echo "FINISHED WITH $id, slept for $seconds_to_sleep seconds"
}

function sleep_and_print_failing_on_yoda() {
    local id="$1"
    local seconds_to_sleep=${2:-0.5}
    sleep "$seconds_to_sleep"
    echo "FINISHED WITH $id, slept for $seconds_to_sleep seconds"

    # after having slept, we fail if id is yoda :
    if [ "$id" == "yoda" ]
    then
        echo "About to fail because Yoda"
        return 1
    fi
}


# array of persons to print :
declare -a force_wielders=(\
    luke\
    leia\
    yoda\
    anakin\
    palpatine\
    kenobi\
)

export force_wielders

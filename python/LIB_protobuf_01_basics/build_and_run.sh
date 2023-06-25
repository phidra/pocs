#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

venv="${this_script_parent}/NOGIT_venv"
[ ! -d "$venv" ] && /usr/bin/env python3 -m venv  "$venv"


if ! "$venv"/bin/python -c 'import google.protobuf' &> /dev/null
then
    "$venv"/bin/python -m pip install protobuf
fi


# if need be :
#     sudo apt install protobuf-compiler
protoc --python_out=$(pwd) addressbook.proto
"$venv"/bin/python run.py

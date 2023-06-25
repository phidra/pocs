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

FILENAME="${this_script_parent}/NOGIT_database.pb"
rm -f "$FILENAME"

echo ""
echo "=== calling protoc to generate code"
# if need be :
#     sudo apt install protobuf-compiler
protoc --python_out=$(pwd) addressbook.proto

echo ""
echo "=== calling writer"
"$venv"/bin/python step1_writer.py "$FILENAME"

echo ""
echo "=== calling reader"
"$venv"/bin/python step2_reader.py "$FILENAME"

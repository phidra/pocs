#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF
To run POC :
    pip install pybind11
    pip install -e .
    python main.py

EOF

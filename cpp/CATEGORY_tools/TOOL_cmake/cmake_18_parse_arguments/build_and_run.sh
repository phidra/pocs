#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cmake -P parse_arguments.cmake

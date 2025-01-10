#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cmake -P pouet.cmake

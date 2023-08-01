#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


rustc entrypoint.rs && ./entrypoint

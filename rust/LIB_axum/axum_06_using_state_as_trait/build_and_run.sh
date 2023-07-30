#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cargo test
cargo run

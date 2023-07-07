#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace


cargo test
cargo run -- "./my_super_file.txt"

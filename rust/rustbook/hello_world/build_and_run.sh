#!/bin/bash

set -o errexit
set -o nounset
set -o xtrace

rustc main.rs -o NOGIT_bin
./NOGIT_bin

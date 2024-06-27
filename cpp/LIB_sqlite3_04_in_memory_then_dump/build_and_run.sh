#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null

rm -rf NOGIT_build
 CXX=clang++-14 meson setup NOGIT_build/ .
 CXX=clang++-14 meson compile  -C NOGIT_build/
DB="${this_script_parent}/NOGIT_pouet.db"
NOGIT_build/bin "$DB"

# en alternative :
# g++ main.cpp -lsqlite3 && ./a.out


echo ""
echo ""
set -o xtrace
sqlite3 "$DB" "SELECT * FROM persons LIMIT 5;"

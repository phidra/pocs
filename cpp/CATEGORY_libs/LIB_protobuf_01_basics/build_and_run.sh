#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


FILENAME="${this_script_parent}/NOGIT_database.pb"
rm -f "$FILENAME"

echo ""
echo "=== calling protoc to generate code"
# if need be :
#     sudo apt install protobuf-compiler
protoc --cpp_out=$(pwd) addressbook.proto

echo ""
echo "=== building"
bin="./NOGIT_bin"
rm -f "$bin"
# NOTE : it's important not to forget pthread support, or else we'll get an (obscure) std::system_exception
g++ -std=c++17 -Wall main.cpp addressbook.pb.cc -pthread -lprotobuf -o "$bin"
chmod +x "$bin"

echo ""
echo "=== running"
"$bin"

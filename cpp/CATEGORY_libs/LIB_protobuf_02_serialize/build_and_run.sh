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
bin_writer="./NOGIT_bin_writer"
bin_reader="./NOGIT_bin_reader"
rm -f "$bin_writer"
rm -f "$bin_reader"
# NOTE : it's important not to forget pthread support, or else we'll get an (obscure) std::system_exception
g++ -std=c++17 -Wall main_writer.cpp addressbook.pb.cc -pthread -lprotobuf -o "$bin_writer"
g++ -std=c++17 -Wall main_reader.cpp addressbook.pb.cc -pthread -lprotobuf -o "$bin_reader"
chmod +x "$bin_writer"
chmod +x "$bin_reader"

FILENAME="${this_script_parent}/NOGIT_database.pb"
rm -f "$FILENAME"

echo ""
echo "=== running writer"
"$bin_writer" "$FILENAME"

echo ""
echo "=== running reader"
"$bin_reader" "$FILENAME"

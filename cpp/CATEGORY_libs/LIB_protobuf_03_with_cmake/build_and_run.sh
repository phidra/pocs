#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

# NOTE : this POC used this article :
# https://blog.conan.io/2019/03/06/Serializing-your-data-with-Protobuf.html


FILENAME="${this_script_parent}/NOGIT_database.pb"
rm -f "$FILENAME"


echo ""
echo "=== building WRITER :"
WRITER_DIR="$this_script_parent/writer"
WRITER_BUILD_DIR="$WRITER_DIR/_build"
WRITER_CMAKE_ROOT_DIR="$WRITER_DIR"
echo "WRITER_BUILD_DIR=$WRITER_BUILD_DIR"
echo "WRITER_CMAKE_ROOT_DIR=$WRITER_CMAKE_ROOT_DIR"

pushd "$WRITER_CMAKE_ROOT_DIR"
rm -rf "$WRITER_BUILD_DIR"
mkdir -p "$WRITER_BUILD_DIR"
conan install --install-folder="$WRITER_BUILD_DIR" .
cmake -B"$WRITER_BUILD_DIR" -H"$WRITER_CMAKE_ROOT_DIR"
popd
make -j -C "$WRITER_BUILD_DIR"


echo ""
echo "=== building READER :"
READER_DIR="$this_script_parent/reader"
READER_BUILD_DIR="$READER_DIR/_build"
READER_CMAKE_ROOT_DIR="$READER_DIR"
echo "READER_BUILD_DIR=$READER_BUILD_DIR"
echo "READER_CMAKE_ROOT_DIR=$READER_CMAKE_ROOT_DIR"

pushd "$READER_CMAKE_ROOT_DIR"
rm -rf "$READER_BUILD_DIR"
mkdir -p "$READER_BUILD_DIR"
conan install --install-folder="$READER_BUILD_DIR" .
cmake -B"$READER_BUILD_DIR" -H"$READER_CMAKE_ROOT_DIR"
popd
make -j -C "$READER_BUILD_DIR"


echo ""
echo "=== running writer"
"$WRITER_BUILD_DIR/bin/writer"  "$FILENAME"


echo ""
echo "=== running reader"
"$READER_BUILD_DIR/bin/reader"  "$FILENAME"

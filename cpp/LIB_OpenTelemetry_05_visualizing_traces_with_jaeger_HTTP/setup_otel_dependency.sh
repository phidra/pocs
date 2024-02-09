#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

pwd_before="$(pwd)"

OTEL_DEP_DIR="${this_script_parent}/NOGIT_DEP_OpenTelemetry"
OTEL_INSTALL_DIR="${OTEL_DEP_DIR}/installed"

set -o xtrace

mkdir -p "$OTEL_DEP_DIR"
mkdir -p "$OTEL_INSTALL_DIR"

cd "$OTEL_DEP_DIR"
git clone --recursive https://github.com/open-telemetry/opentelemetry-cpp
cd opentelemetry-cpp
mkdir -p build
cd build

# OpenTelemetry peut exporter ses traces vers jaeger soit avec gRPC, soit avec HTTP.
# Pour cette POC, je choisis HTTP afin de ne pas avoir à gérer la dépendance à une lib gRPC :
cmake \
    -DWITH_OTLP_GRPC=OFF \
    -DWITH_OTLP_HTTP=ON \
    -DBUILD_TESTING=OFF \
    -DCMAKE_INSTALL_PREFIX="${OTEL_INSTALL_DIR}" \
    ..

cmake --build . --target all
cmake --install .

# le répertoire OTEL_INSTALL_DIR contient maintenant la librairie OTEL, prête à être utilisée par la POC

cd "$pwd_before"

FROM debian:12.5-slim as build

# dépendances nécessaires au compile-time :
RUN apt-get update && \
    apt-get install -y  \
        git \
        ca-certificates \
        build-essential \
        cmake \
        meson \
        ninja-build \
        clang \
        protobuf-compiler \
        protobuf-compiler-grpc \
        libprotobuf-dev \
        libgrpc++-dev \
        libcurl4-openssl-dev \
        libboost-date-time-dev \
    && \
    rm -rf /var/lib/apt/lists/*

RUN \
    mkdir -p /opentelemetry/installed && cd /opentelemetry && \
    git clone --depth=1 --recursive --shallow-submodules --branch=v1.14.0 https://github.com/open-telemetry/opentelemetry-cpp && \
    cd opentelemetry-cpp && \
    mkdir build && cd build && \
    cmake \
        -DCMAKE_INSTALL_PREFIX="/opentelemetry/installed" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE  \
        -DBUILD_TESTING=OFF \
        -DBUILD_SHARED_LIBS=ON \
        -DWITH_OTLP_GRPC=ON \
        -DWITH_OTLP_HTTP=ON \
        .. && \
    cmake --build . --target all && \
    cmake --install .

COPY src /poc_src
RUN \
    CXX="clang++" meson setup /builddir/ /poc_src --buildtype=release && \
    meson compile -C /builddir

# dépendances nécessaires au runtime uniquement :
RUN apt-get install -y libgrpc++1.51 libcurl4

# on a besoin d'utiliser les librairies opentelemetry buildées :
ENV LD_LIBRARY_PATH="/opentelemetry/installed/lib/"

ENTRYPOINT ["/builddir/bin"]



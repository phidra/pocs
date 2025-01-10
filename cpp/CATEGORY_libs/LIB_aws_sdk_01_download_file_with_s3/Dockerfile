FROM debian:12.5-slim as build

# dépendances nécessaires au compile-time :
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        git \
        ca-certificates \
        build-essential \
        cmake \
        meson \
        ninja-build \
        clang \
        libcurl4-openssl-dev \
        zlib1g-dev \
        libssl-dev \
    && \
    rm -rf /var/lib/apt/lists/*


# Paramètres du build : https://github.com/aws/aws-sdk-cpp/blob/main/docs/CMake_Parameters.md
RUN \
    mkdir /aws_sdk_installed && \
    mkdir /aws_sdk_build && cd /aws_sdk_build && \
    git clone --depth=1 --recursive --shallow-submodules --branch="1.11.296" https://github.com/aws/aws-sdk-cpp && \
    cd aws-sdk-cpp && mkdir build && cd build && \
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX=/aws_sdk_installed \
        -DBUILD_ONLY="s3" \
        -DENABLE_TESTING=OFF \
        .. && \
    cmake --build . && \
    cmake --install .


COPY src /poc_src
RUN \
    CXX="clang++" meson setup /builddir/ /poc_src --buildtype=release && \
    meson compile -C /builddir

# dépendances nécessaires au runtime uniquement :
RUN apt-get install -y libcurl4 ca-certificates

# on a besoin d'utiliser les librairies du SDK buildées :
ENV LD_LIBRARY_PATH="/aws_sdk_installed/lib/"

ENTRYPOINT ["/builddir/bin"]


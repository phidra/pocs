#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

Cette POC montre comment utiliser un clang "local" (qui n'est pas system-wide)
Elle télécharge clang-9 (si besoin), et démontre les options à passer pour l'utiliser.
Les difficultés tournent autour de l'utilisation de libc++ :
    https://libcxx.llvm.org/docs/UsingLibcxx.html#using-libc-on-linux
En effet, il faut utiliser la libc++ "locale" aussi (car celle du système n'est pas forcément C++17-compliant)

NOTE = j'ai aussi essayé d'utiliser le linker de LLVM (-fuse-ld=ldd), mais ça semble inutile :
    /path/to/clang-9 -std=c++17 -stdlib=libc++ -fuse-ld=lld /tmp/main17.cpp -lc++ -lc++abi -o /tmp/bin17

EOF


# STEP 1 = on télécharge un version locale de clang (ici, clang-9) :
# ==================================================================

FOLDER_NAME="clang+llvm-9.0.1-x86_64-linux-gnu-ubuntu-16.04"
ARCHIVE_NAME="${FOLDER_NAME}.tar.xz"
PREFIXED_ARCHIVE="NOGIT_${ARCHIVE_NAME}"

DOWNLOAD_URL="https://github.com/llvm/llvm-project/releases/download/llvmorg-9.0.1/${ARCHIVE_NAME}"

PREFIXED_FOLDER="NOGIT_${FOLDER_NAME}"
CLANG_BINARY="${PREFIXED_FOLDER}/bin/clang-9"

if [ -f "${CLANG_BINARY}" ]
then
    echo ""
    echo "+++ Not downloading+extracting again clang-9, using already existing : ${PREFIXED_ARCHIVE}"
else
    # comme on s'apprête à télécharger un gros binaire (clang-9), on demande confirmation avant :
    echo ""
    echo "+++ About to download clang-9 from this URL : ${DOWNLOAD_URL}"
    echo "The file is pretty big, please hit Ctrl+C NOW if you want to abort, or press Enter to continue..."
    read

    echo ""
    echo "Downloading clang-9 from URL : ${DOWNLOAD_URL}"
    wget "${DOWNLOAD_URL}"
    mv "${ARCHIVE_NAME}" "${PREFIXED_ARCHIVE}"

    echo "Extracting clang-9 to : ${PREFIXED_FOLDER}"
    tar -xvJf "${PREFIXED_ARCHIVE}"
    mv "${FOLDER_NAME}" "${PREFIXED_FOLDER}"
    ls -lh "${CLANG_BINARY}"
fi

echo "OK"


# STEP 2 = on l'utilise pour compiler un main c++17 :
# ===================================================

echo ""
echo "+++ Building"

# l'intérêt de la POC est là = voir les options nécessaires pour utiliser le clang local :
"${CLANG_BINARY}" \
    -std=c++17 \
    -stdlib=libc++ \
    main.cpp \
    -lc++ \
    -lc++abi \
    -o NOGIT_bin
# NOTE : éventuellement, "-lm" pourra être nécessaire

# STEP 3 = on runne le binaire (nécessite d'utiliser la stdlib locale via LD_LIBRARY_PATH) :
# ==========================================================================================

echo ""
echo "+++ Running"

LD_LIBRARY_PATH="${PREFIXED_FOLDER}/lib/" ./NOGIT_bin

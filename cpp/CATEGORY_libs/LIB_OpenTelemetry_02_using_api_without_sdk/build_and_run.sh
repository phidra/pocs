#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"

# On récupère les includes d'OpenTelemetry C++ API (header-only) depuis le github :
OTELCPP_CLONE="${this_script_parent}/NOGIT_clone_of_opentelemetry-cpp"
if [ ! -e "$OTELCPP_CLONE" ]
then
    git clone https://github.com/open-telemetry/opentelemetry-cpp "$OTELCPP_CLONE"
fi


# On ne copie que les includes :
OTELCPP_INCLUDES="${this_script_parent}/NOGIT_otel_includes"
mkdir -p "$OTELCPP_INCLUDES"
cp -R "${OTELCPP_CLONE}/api/include/opentelemetry"  "$OTELCPP_INCLUDES"


echo "Using opentelemetry-cpp includes from here : $OTELCPP_INCLUDES"

rm -rf NOGIT_build_dir
 CXX=clang++-14 meson setup NOGIT_build_dir/ .
 CXX=clang++-14 meson compile  -C NOGIT_build_dir/
NOGIT_build_dir/bin


cat << EOF

Cette POC vise à montrer qu'on peut utiliser l'API opentelemetry-cpp sans avoir à compiler la lourde dépendance.
Ici, on se contente d'utiliser l'API C++ via des includes header-only, et on n'utilise pas le SDK.

Conséquence 1 = pas besoin de builder et linker le SDK.

Conséquence 2 = le tracer OTel qu'on utilise est un stub vide qui ne fait rien ; la création d'un span est un noop

EOF

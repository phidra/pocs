#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


rm -rf NOGIT_build_dir
 CXX=clang++-14 meson setup NOGIT_build_dir/ .
 CXX=clang++-14 meson compile  -C NOGIT_build_dir/

# Lancement avec le tracer noop :
NOGIT_build_dir/bin

# Lancement avec le vrai tracer du SDK :
NOGIT_build_dir/bin  "with-sdk-tracer"


cat << EOF

Cette POC montre qu'on peut découpler l'instrumentation d'une app avec l'API OpenTelemetry et l'export des telemetry-data.

Ici, notre app NE DÉPEND PAS du SDK opentelemetry-cpp, elle ne dépend que de l'API header-only.
Elle est utilisée au sein d'un main QUI DÉPEND du SDK (via une sous-lib), et qui peut ou non initialiser un tracer.

Selon que le tracer ait été initialisé ou non, les mêmes appels à l'API d'instrumentation dans l'app produiront des résultats différents :
    - si le tracer a été initialisé : ils enregistreront des spans, affichées sur stdout
    - sinon : ils ne feront rien, les appels sont des noop

Le choix d'initialiser le tracer ou non est fait au runtime (selon qu'on passe un argument ou non au main)

EOF

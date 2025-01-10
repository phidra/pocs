#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


# PRÉREQUIS = avoir buildé la dépendance OTel avec ./setup_otel_dependency.sh

rm -rf NOGIT_build_dir
 CXX=clang++-14 meson setup NOGIT_build_dir/ .
 CXX=clang++-14 meson compile  -C NOGIT_build_dir/

# Lancement avec le tracer noop :
NOGIT_build_dir/bin

# Lancement avec le vrai tracer du SDK :
NOGIT_build_dir/bin  "with-sdk-tracer"


cat << EOF

Cette POC montre qu'on peut découpler l'instrumentation d'une app avec l'API OpenTelemetry et l'export des telemetry-data.

Ici, notre app est une librairie statique qui NE DÉPEND PAS du SDK opentelemetry-cpp, elle ne dépend que de l'API header-only.

Une deuxième librairie statique DÉPEND DU SDK, et est responsable d'initialiser un "vrai" tracer (plutôt que le stub).

Ces deux librairies sont utilisées par le main, qui ne dépend donc directement ni de l'API ni du SDK.

Selon que le tracer ait été initialisé ou non, les mêmes appels à l'API d'instrumentation dans l'app produiront des résultats différents :
    - si le tracer a été initialisé : ils enregistreront des spans, affichées sur stdout
    - sinon : ils ne feront rien, les appels sont des noop

Le choix d'initialiser un vrai tracer avec le SDK, ou de garder le stub noop de l'API est fait au runtime (selon qu'on passe un argument ou non au main)

EOF

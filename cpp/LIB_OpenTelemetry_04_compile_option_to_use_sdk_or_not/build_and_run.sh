#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


rm -rf NOGIT_build_dir

# on choisit au configure-time d'utiliser ou non opentelemetry.
# Décommenter l'une des deux lignes suivantes :
CXX=clang++-14 meson setup -Dcompile_with_opentelemetry=true NOGIT_build_dir/ .
# CXX=clang++-14 meson setup -Dcompile_with_opentelemetry=false NOGIT_build_dir/ .
#

 # Dans les deux cas, le choix d'utiliser OTel ou non a été fait au configure-time, et la compilation+runtime ne change pas :
 CXX=clang++-14 meson compile -C NOGIT_build_dir/
NOGIT_build_dir/bin


cat << EOF

Cette POC montre qu'on peut ne dépendre du SDK OpenTelemetry qu'en fonction d'une option meson passée au configure-time.

L'idée est que le SDK OpenTelemetry C++ est une lourde dépendance dont on aimerait se passer en local.
À l'inverse, l'API OpenTelemetry C++ est plus légère (une lib header-only), on peut l'utiliser systématiquement.
Si on n'utilise que l'API sans le SDK, l'instrumentation du code ne fera rien (le tracer stub fait des noop).
Si on utilise l'API avec le SDK, l'instrumentation du code produira des traces.

Ici, l'app est une librairie statique qui NE DÉPEND PAS du SDK opentelemetry-cpp, elle dépend de l'API header-only.

Une deuxième librairie statique DÉPEND DU SDK, et est responsable d'initialiser un "vrai" tracer (plutôt que le stub).
MAIS cette librairie n'initialisera un vrai tracer que si on a passé la bonne option de configuration à meson.

L'objectif est par exemple de permettre de compiler son projet en local sans utiliser le SDK OpenTelemetry, et de ne
l'utiliser que lorsqu'on compile son projet avec la CI pour livrer dans un environnement observable.

Pour jouer avec la POC, il faut utiliser l'une des deux commandes de configuration (cf. ci-dessus) :

    CXX=clang++-14 meson setup -Dcompile_with_opentelemetry=true NOGIT_build_dir/ .
    CXX=clang++-14 meson setup -Dcompile_with_opentelemetry=false NOGIT_build_dir/ .

EOF

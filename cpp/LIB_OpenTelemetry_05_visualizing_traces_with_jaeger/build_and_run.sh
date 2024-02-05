#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail


# PRÉREQUIS = avoir buildé la dépendance OTel avec ./setup_otel_dependency.sh

rm -rf NOGIT_build_dir
CXX=clang++-14 meson setup NOGIT_build_dir/ .
CXX=clang++-14 meson compile -C NOGIT_build_dir/


cat << EOF

Cette POC démontre la visualisation de traces OTel avec jaeger.

Par rapport aux POCS précédentes :

- il faut modifier le setup d'opentelemetry-cpp pour compiler avec le flag -DWITH_OTLP_HTTP=ON
- il faut modifier le build de la POC pour linker les librairies nécessaires (OTel + externes)
- il faut lancer un jaeger all-in-one dans un docker pour réceptionner les traces exportées


Ce script builde la POC mais ne l'exécute pas.
Une fois la dépendance OpenTelemetry et la POC buildées, il faut lancer jaeger dans un terminal externe :
(cf. https://www.jaegertracing.io/docs/1.53/getting-started/ )

    docker run --rm --name jaeger \\
    -e COLLECTOR_ZIPKIN_HOST_PORT=:9411 \\
    -e COLLECTOR_OTLP_ENABLED=true \\
    -p 6831:6831/udp \\
    -p 6832:6832/udp \\
    -p 5778:5778 \\
    -p 16686:16686 \\
    -p 4317:4317 \\
    -p 4318:4318 \\
    -p 14250:14250 \\
    -p 14268:14268 \\
    -p 14269:14269 \\
    -p 9411:9411 \\
    jaegertracing/all-in-one:1.47


Puis, on peut exécuter la POC, ce qui va envoyer quelques traces à jaeger :

    NOGIT_build_dir/bin


Enfin, on peut consulter les traces sur l'IHM de jaeger :

    http://localhost:16686

Choisir le service "POC-OTel", et cliquer sur "Find Traces" pour visualiser les traces que la POC a créées.

EOF

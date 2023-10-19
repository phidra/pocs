#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset

this_script_parent="$(realpath "$(dirname "$0")" )"

cat << EOF

POC de lecture/conversion du format geoparquet avec ogr2ogr.

Cf. la doc du driver :

    https://gdal.org/drivers/vector/parquet.html


Attention : il faut un gdal >= 3.6.2 pour lire du parquet :
    À date d'écriture de la POC, la version de ogr2ogr sur osgeo/gdal:latest est :
        GDAL 3.7.0dev-8ec21bbf7073f9105a2b4bf70adb6dce0d295874, released 2023/02/22
    Et la version sur mon poste ubuntu 22.04 est :
        GDAL 3.4.1, released 2021/12/27
    Du coup, la présente POC utilise une image docker d'un gdal récent.

Le sample geoparquet utilisé provient de :
    https://github.com/opengeospatial/geoparquet/tree/e52ace139a34778331b94251606070ed5259375f/examples
    (https://geojson.io sur le geojson généré par la POC montre que le sample contient un détourage de l'Amérique du Nord)
EOF



# Quelle est la version de gdal sur l'image docker :
docker run --rm -i osgeo/gdal:latest ogrinfo --version


# Conversion du fichier geoparquet en geojson + formattage avec jq :
docker run \
    --rm \
    -i \
    --mount type=bind,src="${this_script_parent}",dst=/poc/ \
    osgeo/gdal:latest \
    ogr2ogr -f GeoJSON /vsistdout/ /poc/example.parquet | jq "."

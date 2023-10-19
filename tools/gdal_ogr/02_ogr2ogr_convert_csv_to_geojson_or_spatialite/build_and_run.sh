#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset

this_script_parent="$(realpath "$(dirname "$0")" )"

cat << EOF

POC illustrant la conversion d'un CSV (comportant une colonne géométrique en WKT) en geojson, puis en base spatialite.

cf. les docs des drivers GDAL :
    https://gdal.org/drivers/vector/csv.html
    https://gdal.org/drivers/vector/sqlite.html
    https://gdal.org/drivers/vector/geojson.html

EOF

# création d'un CSV avec quelques features de type polyline :
# (j'utilise ";" comme separator pour ne pas avoir à échapper les virgules, un GDAL récent a l'option SEPARATOR, mais il se débrouille sans)
cat << EOF > "${this_script_parent}/NOGIT_input.csv"
name;direction;geom
"A86";"east-west";"LINESTRING(2.300229 48.761769, 2.305523 48.763035, 2.307827 48.763451, 2.308567 48.763433, 2.310597 48.762872)"
"N20";"north-south";"LINESTRING(2.307275 48.759221, 2.308702 48.762873, 2.309250 48.763452, 2.309387 48.764555, 2.310622 48.767628)"
EOF

# Conversion en geojson (/vsistdout/ permet d'écrire sur stdout ; à remplacer par un nom de fichier si besoin) :
ogr2ogr -f GeoJSON /vsistdout/  -oo GEOM_POSSIBLE_NAMES=geom -oo KEEP_GEOM_COLUMNS=NO NOGIT_input.csv | jq "."

# Conversion en une base spatialite :
rm -f NOGIT_out.db
ogr2ogr -dsco SPATIALITE=yes -f SQLite NOGIT_out.db  -oo GEOM_POSSIBLE_NAMES=geom -oo KEEP_GEOM_COLUMNS=NO NOGIT_input.csv
sqlite3 NOGIT_out.db "SELECT load_extension('mod_spatialite'); SELECT *, ST_AsText(geom) FROM NOGIT_input;"

# Visualisation de la base spatialite avec dBeaver :
#   - ouvrir NOGIT_out.db avec dBeaver comme base sqlite
#   - SELECT load_extension('mod_spatialite');
#   - SELECT name, direction, ST_AsText(geom_wkt) FROM nogit_input
#   - clic-droit sur la colonne géométrique > View/Format > Set "..." Tranform > Geometry
#   - cliquer sur l'onglet vertical "Spatial" pour consulter ses features

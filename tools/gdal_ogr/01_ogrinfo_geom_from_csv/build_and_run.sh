#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset

this_script_parent="$(realpath "$(dirname "$0")" )"


cat << EOF

POCs tirée de la doc du driver CSV de GDAL :
    https://gdal.org/drivers/vector/csv.html

En résumé, on montre qu'on peut construire des layers géométriques à partir d'un input CSV.

Quelques extraits de la doc :
    CSV files have one line for each feature (record) in the layer (table).
    By default, the driver attempts to treat the first line of the file as a list of field names for all the fields. However, if one or more of the names is all numeric it is assumed that the first line is actually data values and dummy field names are generated internally (field_1 through field_n) and the first record is treated as a feature.
    All CSV files are treated as UTF-8 encoded.

C'est un peu bizarre : au lieu de préciser le field qui contient les points X et Y, on contient une liste de fields "possibles"
(mais le cas où la liste n'a qu'un élément se dégénère en le cas intuitif)

man ogrinfo
    -ro               Open the data source in read-only mode.
    -al               List all features of all layers (used instead of having to give layer names as arguments).
    -oo NAME=VALUE    Dataset open option (format-specific)

========================================================================================================================

EOF


# chaque record CSV contient un point :
cat << EOF > "${this_script_parent}/NOGIT_input_point.csv"
Latitude,Longitude,Name
48.1,0.25,"First point"
49.2,1.1,"Second point"
47.5,0.75,"Third point"
EOF

ogrinfo -ro -al NOGIT_input_point.csv -oo X_POSSIBLE_NAMES=Longitude -oo Y_POSSIBLE_NAMES=Latitude -oo KEEP_GEOM_COLUMNS=NO


echo "========================================================================================================================"

# chaque record CSV contient un point, en agrégeant plusieurs records, on forme une polyline :
cat << EOF > "${this_script_parent}/NOGIT_input_points_forming_a_polyline.csv"
way_id,pt_id,x,y
"Autoroute du Soleil",1,0,0
"Autoroute du Soleil",2,1,0
"Autoroute du Soleil",3,2,0
"Chemin de Traverse",1,9,9
"Chemin de Traverse",2,8,8
"Chemin de Traverse",3,7,7
EOF

ogrinfo NOGIT_input_points_forming_a_polyline.csv -dialect SQLite -sql "SELECT way_id, MakeLine(MakePoint(CAST(x AS float),CAST(y AS float))) FROM NOGIT_input_points_forming_a_polyline GROUP BY way_id"



echo "========================================================================================================================"

# chaque record CSV contient une polyline (j'utilise ";" comme separator pour ne pas avoir à échapper les virgules) :
cat << EOF > "${this_script_parent}/NOGIT_input_polyline.csv"
way_id;geometry
"Autoroute du Soleil";LINESTRING(0 0, 1 0, 2 0)
"Chemin de Traverse";LINESTRING(9 9, 8 8, 7 7)
EOF

ogrinfo -ro -al NOGIT_input_polyline.csv -oo SEPARATOR=SEMICOLON -oo GEOM_POSSIBLE_NAMES=geometry -oo KEEP_GEOM_COLUMNS=NO

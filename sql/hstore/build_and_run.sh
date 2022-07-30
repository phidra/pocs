#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset


cat << NOTES
cf. la doc postgres pour la liste complète des opérations possibles :
    https://www.postgresql.org/docs/9.5/hstore.html
Notamment, il y a possibilité de convertir la liste des tags en JSON.
En plus d'utiliser une column de type HSTORE, on peut utiliser une colonne de type TEXT en la castant en ::hstore
Un tuto ici :
    https://www.postgresqltutorial.com/postgresql-hstore/
NOTES


# BDD params :
host=localhost
port=5432
username=pouet
password=pouet
db_name=testing_hstore

echo ""
echo "BDD params :"
echo "  host           = $host"
echo "  port           = $port"
echo "  username       = $username"
echo "  password       = $password"
echo "  db_name        = $db_name"
echo ""

# DATABASE CREATION (needs propre roles) :
set +o errexit
if ! PGPASSWORD="${password}"  createdb -h "$host" -p "$port" -U "$username" "$db_name" -e
then
    echo ""
    echo "ERROR : createdb failed, maybe because of an already existing database ?"
    echo "        if necessary, you can run :"
    echo ""
    echo "        dropdb -h \"$host\" -p \"$port\" -U \"$username\" \"$db_name\" -e"
    exit 2
fi
set -o errexit


PGPASSWORD="$password"  psql -h "$host" -p "$port" -d "$db_name" -U "$username" -e  --file="poc_hstore.sql"

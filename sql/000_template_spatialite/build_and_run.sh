#!/bin/bash

set -o errexit
set -o pipefail
set -o nounset

rm -f NOGIT_sqlite_poc_database.db
sqlite3 NOGIT_sqlite_poc_database.db < poc.sql

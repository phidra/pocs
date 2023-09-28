-- abort on first error :
.bail on
.mode table
.echo off
.trace off


-- POC :
.print ""
.print "========================================================================================================================"
.print "L'objectif de la POC est de créer une table géométrique spatialite à partir d'une colonne contenant la géométrie WKT"
.print ""
.print "Dès avant ajout de la colonne géométrique spatialite, chaque ligne de la table contient la géométrie en WKT dans une colonne textuelle."
.print "Une fois la colonne géométrique créée, une instruction UPDATE utilise la colonne textuelle WKT pour feeder la colonne géométrique."
.print ""
.print "RÉSULTAT = tout fonctionne correctement (attention : le SRID doit être cohérent)."
.print "========================================================================================================================"
.print ""
.print ""


-- Création d'une table d'autoroutes :
.print ""
CREATE TABLE autoroutes (name VARCHAR, length_km INTEGER, wkt VARCHAR);
INSERT INTO autoroutes VALUES ('A86', 79, 'LINESTRING(0 0, 1 0, 2 0)');
INSERT INTO autoroutes VALUES ('A10', 557, 'LINESTRING(9 9, 8 8, 7 7)');

SELECT * FROM autoroutes;

.output /dev/null
SELECT load_extension('mod_spatialite');
SELECT InitSpatialMetaData(TRUE);
.output

SELECT AddGeometryColumn('autoroutes', 'geom', 4326, 'LINESTRING', 'XY', TRUE);
SELECT name, length_km, AsText(geom) FROM autoroutes;

UPDATE autoroutes SET geom = ST_GeomFromText(wkt, 4326);
.print ""
SELECT name, length_km, AsGeojson(geom) FROM autoroutes;

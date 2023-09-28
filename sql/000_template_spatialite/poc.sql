-- abort on first error :
.bail on
.mode table
.echo off
.trace off


-- POC :
.print ""
.print "========================================================================================================================"
.print "L'objectif de la POC est de TODO TODO TODO"
.print "- TODO"
.print "- TODO"
.print "- TODO"
.print "========================================================================================================================"
.print ""
.print ""


-- Création d'une table d'autoroutes :
.print ""
CREATE TABLE autoroutes (name VARCHAR, length_km INTEGER, wikipedia_link VARCHAR);
INSERT INTO autoroutes VALUES ('A86', 79, 'https://fr.wikipedia.org/wiki/Autoroute_A86_(France)');
INSERT INTO autoroutes VALUES ('A10', 557, 'https://fr.wikipedia.org/wiki/Autoroute_A10_(France)');
INSERT INTO autoroutes VALUES ('A6', 445, 'https://fr.wikipedia.org/wiki/Autoroute_A6_(France)');

SELECT * FROM autoroutes;

-- Deux instructions à exécuter avant toute utilisation de spatialite :
.print ""
.output /dev/null
SELECT load_extension('mod_spatialite');
SELECT InitSpatialMetaData(TRUE);
.output

-- On peut très bien ajouter une colonne géométrique a posteriori, sur une table existante :
-- (TRUE final ajoute une contrainte interdisant à la géométrie d'être NULL... mais elle peut être vide quand même)
SELECT AddGeometryColumn('autoroutes', 'geom', 4326, 'LINESTRING', 'XY', TRUE);
UPDATE autoroutes SET geom = ST_GeomFromText('LINESTRING(0 0, 1 0, 2 0)', 4326)  WHERE name = 'A86';
UPDATE autoroutes SET geom = ST_GeomFromText('LINESTRING(9 9, 8 8, 7 7)', 4326)  WHERE name = 'A10';

.print ""
SELECT name, length_km, wikipedia_link, AsText(geom) FROM autoroutes;

-- abort on first error :
.bail on
.mode table
.echo off
.trace off


-- POC :
.print ""
.print "========================================================================================================================"
.print "L'objectif de la POC est de créer une table géométrique spatialite manuellement."
.print "Cette POC apprend plein de choses :"
.print "- deux instructions sont indispensable avant création d'une table :"
.print "    SELECT load_extension('mod_spatialite');"
.print "    SELECT InitSpatialMetaData(TRUE);"
.print "    (ne pas oublier TRUE pour la deuxième sinon c'est archi archi lent)"
.print "- on peut créer une colonne géométrique a posteriori sur une table existante et non-vide (les lignes existantes ont alors une géométrie vide)"
.print "- on peut interdire des géométries nulles (mais sachant qu'elles peuvent être vides, l'intérêt est faible)"
.print "- une fois une colonne géométrique ajoutée, on peut alors updater chaque ligne pour setter sa géométrie"
.print "- quand on veut setter une géométrie pour une ligne, la géométrie doit être définie dans le même SRID que la colonne"
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

-- Sur cette table existante, ajout d'une colonne géométrique :

-- Deux instructions à exécuter avant toute utilisation de spatialite :
.print ""
.output /dev/null
SELECT load_extension('mod_spatialite');
SELECT InitSpatialMetaData(TRUE);  -- si on oublie TRUE, ce sera démesurément lent (j'ai du killer après ~10 minutes)
.output

-- On peut très bien ajouter une colonne géométrique a posteriori, sur une table existante :
-- (TRUE final ajoute une contrainte interdisant à la géométrie d'être NULL... mais elle peut être vide quand même)
SELECT AddGeometryColumn('autoroutes', 'geom', 4326, 'LINESTRING', 'XY', TRUE);
.print ""
SELECT name, length_km, wikipedia_link, AsText(geom) FROM autoroutes;

-- définition VALIDES de géométries :
UPDATE autoroutes SET geom = ST_GeomFromText('LINESTRING(0 0, 1 0, 2 0)', 4326)  WHERE name = 'A86';
UPDATE autoroutes SET geom = ST_GeomFromText('LINESTRING(9 9, 8 8, 7 7)', 4326)  WHERE name = 'A10';

-- définitions INVALIDES de géométries :
-- interdit par la contrainte sur la colonne géométrique :
-- UPDATE autoroutes SET geom = NULL WHERE name = 'A6';
-- il est interdit de ne pas préciser le SRID :
-- UPDATE autoroutes SET geom = ST_GeomFromText('LINESTRING(0 0, 1 0, 2 0)')  WHERE name = 'A86';

.print ""
SELECT name, length_km, wikipedia_link, AsText(geom) FROM autoroutes;

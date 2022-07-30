CREATE EXTENSION hstore;

-- Création d'une table avec un champ de type HSTORE :
CREATE TABLE knights (name VARCHAR, tags HSTORE);
INSERT INTO knights VALUES ('arthur', '"role" => "king", "class" => "warrior"');
INSERT INTO knights VALUES ('merlin', '"role" => "counsellor", "class" => "mage"');
INSERT INTO knights VALUES ('lancelot', '"role" => "peon"');

-- Requêter les rows qui ont un certain tag :
SELECT * FROM knights WHERE tags ? 'role';
--      name   |                 tags
--   ----------+---------------------------------------
--    arthur   | "role"=>"king", "class"=>"warrior"
--    merlin   | "role"=>"counsellor", "class"=>"mage"
--    lancelot | "role"=>"peon"
SELECT * FROM knights WHERE tags ? 'class';
--     name  |                 tags
--   --------+---------------------------------------
--    arthur | "role"=>"king", "class"=>"warrior"
--    merlin | "role"=>"counsellor", "class"=>"mage"


-- Afficher la valeur d'un tag en particulier dans la réponse :
SELECT name, tags->'role' AS role, tags->'class' AS class FROM knights;
--      name   |    role    |  class
--   ----------+------------+---------
--    arthur   | king       | warrior
--    merlin   | counsellor | mage
--    lancelot | peon       |



-- Ajout d'un tag :
UPDATE knights SET tags = tags || '"roxxor" => "yes"'::hstore WHERE name = 'merlin';
SELECT * FROM knights WHERE tags ? 'roxxor';
--     name  |                          tags
--   --------+--------------------------------------------------------
--    merlin | "role"=>"counsellor", "class"=>"mage", "roxxor"=>"yes"

-- suppression d'un tag :
UPDATE knights SET tags = delete(tags, 'roxxor') WHERE name = 'merlin';
SELECT * FROM knights WHERE tags ? 'roxxor';
--    name | tags
--   ------+------

-- requête par un couple clé-valeur particulier :
SELECT * FROM knights WHERE tags @> '"role" =>    "king"';
--     name  |              tags
--   --------+--------------------------------
--    arthur | "role"=>"king", "class"=>"yes"

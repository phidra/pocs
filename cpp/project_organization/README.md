CONTEXTE = fin 2023, par ces POCs, je veux montrer une structure "canonique" pour un projet C++.

Je les ai découpées en plusieurs morceaux pour illustrer les concepts progressivement.

(NOTE : les considérations liées à l'archi hexagonale ont été laissées de côté ; elles sont plutôt à exprimer en terme de dépendances des composants les uns envers les autres)

**POC 1 avec un unique composant pour illustrer la structure de base** :

- coder l'essentiel de son projet sous la forme d'une lib, et le main est un "simple utilisateur" de la lib
- organiser sa lib en 4 répertoires :
    ```
    myproject/mylib
    ├── impl    = l'implémentation privée
    ├── mains   = les mains du composant
    ├── public  = l'API publique
    └── tests   = les tests
    ```
- plus en détail, ça donne :
    ```
    myproject/mylib
    ├── impl
    │   ├── private_thing.cpp
    │   ├── private_thing.h
    │   ├── public_thing1.cpp
    │   └── public_thing2.cpp
    ├── mains
    │   └── mylib_cli.cpp
    ├── public
    │   └── mylib
    │       ├── public_thing1.h
    │       └── public_thing2.h
    └── tests
        ├── test_mylib_public_api.cpp
        └── test_private_thing.cpp
    ```
- les utilisateurs externes de la lib n'ont accès qu'à `public`, donc à `public_thing1.h` et `public_thing2.h` (et notamment, ils n'ont pas accès à `private_thing.h`, sous `impl`)
- pour que l'inclusion d'un header public se fasse en préfixant par le nom du composant, on complique artificiellement le chemin :
    ```
    mylib/public/mylib/my_header.h
    ```
- le répertoire `public` ne contient pas _tous_ les headers de la librairie : juste ceux qui sont publics ! (ici, `private_thing.h` est un header privé)
- on distingue deux types de tests :
    - tests de l'API publique = le test un main qui est "simple utilisateur" de la lib
    - tests de trucs privés = le test a des super-pouvoirs : il a accès à des trucs internes à la lib (ici, `private_thing.h`), inaccessibles aux "simples utilisateurs"
- dans l'esprit, les tests de l'API publique pourraient même vivre en dehors du composant


**POC 2 avec plusieurs composants pour illustrer les dépendances** :

- on peut définir plusieurs composants = plusieurs libs à la racine du projet
- un composant peut dépendre d'un autre (sidenote : avec meson, il suffit d'ordonner correctement ses `subdir` pour qu'un sous-fichier puisse utiliser ce que définit un sous-fichier précédent)
- chaque lib (= chaque composant) respecte la même structure de base à 4 répertoires, et garde les mêmes propriétés :
    - headers publics vs. implémentation privée
    - tests de l'implémentation privée vs. tests de l'API publique
    - possibilité d'avoir un sub-main propre à chaque composant
- le main principal du projet peut être, au choix :
    - un main à part (ou un répertoire à part) indépendant du reste, simple utilisateur des librairies représentant les autres composants
    - le sub-main propre à l'un des composants, qui joue alors le rôle de "composant principal" du projet

**POC 3 avec des composants imbriqués** :

- au lieu d'avoir une simple liste de composants siblings, la hiérarchie de composants n'est plus flat : certain sont regroupés dans un même répertoire
- note : pour que la hiérarchie nested soit visible lorsqu'on inclus l'un des sous-composants, il faut complexifier artificiellement (encore plus que normalement) le répertoire `public/`
- le répertoire contenant les sous-composants pourrait sans doute être plus qu'un simple container, il pourrait être un "composant parent", avec du code ; ça revient à ajouter le quatuor "habituel" au répertoire parent :
    ```
    ├── child1
    │   ├── impl
    │   ├── mains
    │   ├── public
    │   └── tests
    ├── child2
    │   ├── impl
    │   ├── mains
    │   ├── public
    │   └── tests
    ├── impl
    ├── mains
    ├── public
    └── tests
    ```


Suites éventuelles à donner :

- faire une POC avec un composant parent qui est plus qu'un simple container
- faire les mêmes POC avec cmake plutôt que meson

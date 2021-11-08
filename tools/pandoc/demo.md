---
title: POC = faire des slides avec pandoc
author: MySelf
theme: Frankfurt
colortheme: seahorse
date: 24 décembre 2021
urlcolor: blue
fonttheme: professionalfonts
linkstyle: bold

---

# Contexte

## C'est quoi ?

Cette présentation fait office de démo.

Je peux consulter le markdown directement, ou voir les slides PDF générés.

## Exemple

Exemple de slide, qui utilise markdown classiquement :

- Maître Corbeau, sur un arbre perché,
- Tenait en son bec **un fromage**.
- Maître Renard, par l'odeur alléché,
- Lui tint à peu près ce langage :

Et au passage, [plouf splash glouglou](https://www.youtube.com/watch?v=bTkyejmsbWE)

## Quelques liens

Les liens suivants contiennent des infos :

- à la base, [cette page](https://gist.github.com/johnloy/27dd124ad40e210e91c70dd1c24ac8c8) liste des solutions pour faire des slides
- [la page](https://ashwinschronicles.github.io/beamer-slides-using-markdown-and-pandoc) qui m'a conduit à utiliser pandoc
- [une page](https://learnbyexample.github.io/customizing-pandoc/) avec plein d'infos utiles
- [autre tuto](https://github.com/alexeygumirov/pandoc-beamer-how-to) pandoc + beamer

# Les features

## Thème

Déjà, vous avez vu le super thème ?

Et avec le sommaire et tout et tout ?

Ce sont des thèmes beamer, cf. [ce lien](https://deic-web.uab.cat/~iblanes/beamer_gallery/individual/Frankfurt-seahorse-default.html)

## Images

Voici une image :

![ma super image](./images/dessin.png){ height=230px }

Je n'ai pas trouvé comment la centrer simplement (sauf à accepter d'avoir un caption)...

## Commentaires HTML

Ça ne se voit pas, mais cette slide contient un trésor en commentaire HTML

<!-- le fameux trésor-->

## Plusieurs colonnes :

::: columns

:::: column

contenu colonne 1 :

- pouet
- youpi
- coucou

::::

:::: column

Contenu colonne 2 :

(ça pourrait être une image)

::::

:::

## Police plus petite :

Texte normal

\small

Texte petit 

\tiny

Texte tout petit

\normalsize

Texte normal de nouveau

cf. [ce lien](https://www.overleaf.com/learn/latex/Font_sizes,_families,_and_styles) pour les tailles supportées.

## autres exports que PDF

L'export PDF est fait avec [beamer](https://fr.overleaf.com/learn/latex/Beamer) via latex.

D'autres exports sont possibles, notamment pour permettre des présentations HTML/JS.

Par exemple [slidy](https://www.w3.org/Talks/Tools/Slidy2/#(1)) :

- autoporteur, mais semble assez dégueulasse sans style

    ```
    pandoc -t slidy demo.md -o NOGIT_demo_slidy.html
    ```
- pandoc ne génère que le **BODY** HTML
- il faut copier-coller ce body dans [ce squelette slidy](https://www.w3.org/Talks/Tools/Slidy2/#(3))

Les différents formats de sortie gérés sont dans `man pandoc`

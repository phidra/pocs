#!/usr/bin/env python3
# coding: utf-8

import io

"""
Démonstration d'un pipeline de traitement avec coroutines, comme proposé par :
    http://www.dabeaz.com/coroutines/Coroutines.pdf

Le pipeline est constitué :
    d'une fonction "classique" SOURCE qui feede la première étape du pipeline
    d'une "dernière" étape du pipeline SINK = une coroutine qui absorbe ce qu'elle reçoit
    de N coroutines, qui sont autant d'étapes de pipelines :
        stage1:  ne garde qu'une ligne sur deux
        stage2:  met la première lettre de chaque mot en capitale
        stage3:  supprimer les espaces
    chaque coroutine du pipeline reçoit une valeur via "yield", la modifie, et transmet via "send" la valeur modifiée à la coroutine suivante

keypoint : c'est la SOURCE qui drive le flot de data  : son appel à send trigge toutes les étapes du pipeline.

Dans cette version, le pipeline "absorbe" ce qui lui est passé (et ne renvoie pas de valeurs à l'appelant).
On pourrait imaginer une version où chaque coroutine retourne ce que lui a retourné sa target,
et où c'est du coup l'appelant qui printe, j'ai testé, ça marche aussi.
"""


def stage4_displayer_coroutine():
    while True:
        line = (yield)
        print(line)


def stage1_every_other_line_coroutine(target):
    display = False
    while True:
        line = (yield)
        if display:
            target.send(line)
        display = not display


def stage2_capitalize_coroutine(target):
    do_capitalize = lambda word: word[0].upper() + word[1:]
    while True:
        line = (yield)
        modified_line = " ".join(do_capitalize(word) for word in line.split(" "))
        target.send(modified_line)


def stage3_remove_whitespaces_coroutine(target):
    while True:
        line = (yield)
        target.send(line.replace(" ", "-"))


GEORGE_SAND_LETTER = """Cher ami,
Je suis toute émue de vous dire que j'ai
bien compris l'autre jour que vous aviez
toujours une envie folle de me faire
danser. Je garde le souvenir de votre
baiser et je voudrais bien que ce soit
une preuve que je puisse être aimée
par vous. Je suis prête à montrer mon
affection toute désintéressée et sans cal-
cul, et si vous voulez me voir ainsi
vous dévoiler, sans artifice, mon âme
toute nue, daignez me faire visite,
nous causerons et en amis franchement
je vous prouverai que je suis la femme
sincère, capable de vous offrir l'affection
la plus profonde, comme la plus étroite
amitié, en un mot : la meilleure épouse
dont vous puissiez rêver. Puisque votre>
âme est libre, pensez que l'abandon ou je
vis est bien long, bien dur et souvent bien
insupportable. Mon chagrin est trop
gros. Accourrez bien vite et venez me le
faire oublier. À vous je veux me sou-
mettre entièrement.
Votre poupée
"""

f = io.StringIO(GEORGE_SAND_LETTER)

stage4_displayer = stage4_displayer_coroutine()
stage4_displayer.send(None)
stage3_remove_whitespaces = stage3_remove_whitespaces_coroutine(stage4_displayer)
stage3_remove_whitespaces.send(None)
stage2_capitalize = stage2_capitalize_coroutine(stage3_remove_whitespaces)
stage2_capitalize.send(None)
stage1_every_other_line = stage1_every_other_line_coroutine(stage2_capitalize)
stage1_every_other_line.send(None)

for line in f:
    stage1_every_other_line.send(line.rstrip())

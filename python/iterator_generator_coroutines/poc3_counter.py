#!/usr/bin/env python3
# coding: utf-8

"""
Démonstration d'un compteur collaboratif entre l'appelant et l'appelée (= la coroutine)

Le flot d'exécution est pas forcément intuitif, la simplicité de cette POC aide à le voir.

Ce qu'il faut retenir : dans les deus cas suivants, on a un changement de contexte au niveau du signe "=" :
    x = coro.send(...)
    x = yield ...

Vu depuis l'APPELANT de la coroutine, la ligne suivante fait deux choses successives :
    RETURNED_BY_COROUTINE = coro.send(TO_PASS)
    1. à droite du signe "=" : l'appelant PASSE LA MAIN à la coroutine, en lui TRANSMETTANT le contenu de TO_PASS
    2. à gauche du signe "=" plus tard, quand la COROUTINE décide de rendre la main, ce qu'elle aura renvoyé sera placée dans RETURNED_BY_COROUTINE

Réciproquement, vu depuis l'INTÉRIEUR de la coroutine, la ligne suivante fait deux choses successives :
    PASSED_BY_CALLER = yield TO_RETURN
    1. à droite du signe "=" : la coroutine REND LA MAIN à l'apellant, en lui TRANSMETTANT le contenu de TO_RETURN
    2. à gauche du signe "=" lorsque l'appelant passe de nouveau la main à la coroutine, ce qu'il lui aura transmis sera placé dans PASSED_BY_CALLER

Une autre façon de voir les choses : conceptuellement, vu depuis l'appelant, on pourrait séparer en DEUX étapes :
    étape A. le fait de passer la main à la coroutine en lui transmettant des valeurs
    étape B. le fait de récupérer la main, en récupérant également ce que la coroutine nous a renvoyé

Ce qui donnerait :
    étape A = PASSER_LA_MAIN_A_LA_COROUTINE(CE_QUON_VEUT_TRANSMETTRE_A_LA_COROUTINE)
    étape B = CE_QUE_LA_COROUTINE_NOUS_RENVOIT = REPRENDRE_ICI_LORSQUE_LA_COROUTINE_REND_LA_MAIN()

Mais comme les deux étapes sont indissociables (lorsque la coroutine rend la main, on reprend obligatoirement là où on lui a passé la main) on merge tout ça en une seule ligne :
    CE_QUE_LA_COROUTINE_NOUS_RENVOIT = PASSER_LA_MAIN_A_LA_COROUTINE(CE_QUON_VEUT_TRANSMETTRE_A_LA_COROUTINE)

(et c'est la même chose pour x = coro.send(...) )
"""

def collaborative_counter():
    print(f"CAS 3 = together with my caller, we will count.")
    initial = 0
    print(f"[INSIDE  COROUTINE]  {initial}")

    passed_to_coroutine = yield initial + 1
    print(f"[INSIDE  COROUTINE]  {passed_to_coroutine}")

    passed_to_coroutine = yield passed_to_coroutine + 1
    print(f"[INSIDE  COROUTINE]  {passed_to_coroutine}")

    yield passed_to_coroutine + 1

coro = collaborative_counter()

returned_by_coroutine = coro.send(None)  # initialisation de la coroutine
print(f"[OUTSIDE COROUTINE]  {returned_by_coroutine}")

returned_by_coroutine = coro.send(returned_by_coroutine + 1)
print(f"[OUTSIDE COROUTINE]  {returned_by_coroutine}")

returned_by_coroutine = coro.send(returned_by_coroutine + 1)
print(f"[OUTSIDE COROUTINE]  {returned_by_coroutine}")

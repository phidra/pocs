#!/usr/bin/env python3
# coding: utf-8

"""
APPRIS :
- "appeler" une coroutine n'exécute pas de code (tout comme pour les générateurs), ça créée juste la coroutine.
- une fois créée, la coroutine doit être initialisée par un appel à coro.send(None) (ou son équivalent : next(coro))
- on peut alors alterner le fil d'exécution entre l'appelant et la coroutine avec gen.send / yield
"""

# définition de la coroutine :
def prefixator(prefix):

    # la ligne suivante N'EST PAS exécutée lorsqu'on appelle :     coro = prefixator("LUKE")
    # elle n'est exécutée QUE lorsqu'on appelle le premier send :  coro.send(None)
    print(f"coroutine creation with prefix : {prefix}")  # ceci 
    try:
        while True:
            received = (yield)
            print(f"[{prefix}] {received}")
    except GeneratorExit:
        print("inside coroutine : CLOSING")

# on "l'instancie" (cette ligne n'exécute aucune ligne de la coroutine)
coro = prefixator("LUKE")

# initialise la coroutine (on exécute les lignes jusqu'au yield) :
coro.send(None)
# next(coro)  # ceci marche aussi puisque c'est équivalent


# Alternance du fil d'exécution entre l'appelant et la coroutine :
coro.send("coucou")
print("le")
coro.send("monde")
print("ça")
coro.send("va")
print("bien")
coro.send("?")

print("")
print("About to close the coroutine")
coro.close()  # pas obligatoire : la coroutine sera implicitement détruite quoiqu'il arrive
print("After coroutine closing")
print("")

try:
    coro.send("test ?")
except StopIteration:
    print("coro.close() a raisé GeneratorExit dans la coroutine")
    print("tout appel ultérieur à coro.send() raise StopIteration")

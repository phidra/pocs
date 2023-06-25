#!/usr/bin/env python3
# coding: utf-8

"""
APPRIS :
- lorsque la coroutine ests "épuisée" (elle a retourné), elle raise une StopIteration (tout comme les générateurs)
"""


def limited_prefixator(prefix, limit):
    print(f"INSIDE COROUTINE : creation with prefix '{prefix}' and limit '{limit}'")
    for i in range(limit):
        received = (yield)
        print(f"[{prefix}] {received}")
    print("INSIDE COROUTINE : j'ai fini la boucle, je m'apprête à retourner !")


coro = limited_prefixator("LUKE", 3)
coro.send(None)


coro.send("coucou")
print("Jusqu'ici tout va bien 1")
coro.send("le")
print("Jusqu'ici tout va bien 2")

# lorsque l'appel suivant sera traité par la coroutine, elle sortira de la boucle et retournera.
# Lorsque une coroutine retourne, elle raise StopIteration (tout comme un générateur)
coro.send("monde")
# Le code ci-dessous ne sera pas exécuté (à moins d'attrapper StopIteration plus haut) :
print("Jusqu'ici tout va bien 3")
coro.send("ça")
print("Jusqu'ici tout va bien 4")
coro.send("va")
print("Jusqu'ici tout va bien 5")
coro.send("?")
print("Jusqu'ici tout va bien 6")

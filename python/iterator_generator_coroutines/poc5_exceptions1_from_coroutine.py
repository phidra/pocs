#!/usr/bin/env python3
# coding: utf-8

import io

"""
Que se passe-t-il si la coroutine raise une exception ?
    1. l'exception est propagée à l'appellant, qui peut la traiter
    2. la coroutine devient inutilisable (son flot d'exécution s'est arrêté : elle a raisé)
    du coup, si on essaye de l'utiliser tout de même en lui passant une valeur avec gen.send...
    ... elle raise StopIteration (tout comme une coroutine qui a déjà retourné)
"""


class EmptyLineError(ValueError):
    pass


def prefixator_coroutine(prefix):
    line = ""
    while True:
        line = yield f"[{prefix}]  {line}"
        if not line:
            raise EmptyLineError("empty line !")

prefixator = prefixator_coroutine("POUET")
prefixator.send(None)

for i in range(15):
    try:
        if i == 12:
            prefixed = prefixator.send("")
        else:
            prefixed = prefixator.send(f"this is the line #{i}")
        print(prefixed)
    except EmptyLineError:
        print("EMPTY LINE !")
        continue

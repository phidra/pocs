#!/usr/bin/env python3
# coding: utf-8

import io

"""
Que se passe-t-il si l'appelant veut raiser une exxception DANS la coroutine ?
    on peut la passer avec coro.throw(MyException)
    à l'intérieur de la coroutine, tout se passe comme si la ligne avec yield avait raisé
    la coroutine peut donc traiter l'exception
"""


class InvalidInput(ValueError):
    pass


def prefixator_coroutine(prefix):
    line = ""
    while True:
        try:
            line = yield f"[{prefix}]  {line}"
        except InvalidInput:
            print("[FROM INSIDE COROUTINE] I can't properly prefix an invalid input !")

prefixator = prefixator_coroutine("POUET")
prefixator.send(None)

for i in range(15):
    if i == 12:
        prefixator.throw(InvalidInput("12"))
    else:
        prefixed = prefixator.send(f"this is the line #{i}")
        print(prefixed)

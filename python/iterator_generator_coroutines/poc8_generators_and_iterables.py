#!/usr/bin/env python3
# coding: utf-8


"""
Cette POC montre que les GENERATORS sont à la fois :
    - des ITERABLES (on peut appeler iter dessus + les utiliser dans des for-loops)
    - des ITERATORS (on peut appeler next dessus ce qui renvoie leurs valeurs)

Par ailleurs, lorsque les generators atteignent la fin de leur scope, ils raisent automatiquement StopIteration

Du coup, on peut utiliser les generators dans des for-loops qui wrappent justement ce comportement.

A priori, un generator est un cas particulier de coroutines qui ne fait que renvoyer des valeurs sans jamais en accepter.
"""


def squares_up_to_limit():
    print("-------> currently initializing the generator")
    i = 0
    LIMIT = 30
    while i*i < LIMIT:
        yield i*i
        i += 1
    print("-------> I'm exhausted, I will now raise StopIteration")


# NOTE : ici, on appele next DIRECTEMENT sur le GENERATOR (sans avoir à utiliser iter avant)
# -> les GENERATORS sont donc des ITERATORS
print("\n=== manually iterating on the generator (without calling iter)")
print("=== about to 'call' generator (will do nothing)")
gen = squares_up_to_limit()
print("=== about to manually iterate (calling next) on the generator")
try:
    while True:
        print("[ MANUAL ] next is : ", next(gen))
except StopIteration:
    print("CAUGHT StopIteration !")


# NOTE : ici, on appele next sur ce qu'a renvoyé iter(mongenerator)
# -> les GENERATORS sont donc aussi des ITERABLES (vu qu'iter(generator) renvoie un ITERATOR)
print("\n=== manually iterating on the generator (with calling iter)")
print("=== about to 'call' generator (will do nothing)")
gen2 = squares_up_to_limit()
print("=== about to 'iter(generator)' (will do nothing)")
result = iter(gen2)
print("=== about to manually iterate (calling next) on the result of iter(generator)")
try:
    while True:
        print("[ MANUAL ] next is : ", next(result))
except StopIteration:
    print("CAUGHT StopIteration !")


# comme les generator sont des iterables, on peut les utiliser dans une for-loop :
print("\n=== iterating on the generator using a for-loop")
print("=== about to 'call' generator (will do nothing)")
gen3 = squares_up_to_limit()
print("=== about to iterate on the generator using for-loop")
for i in gen3:
    print("[FOR-LOOP] next is : ", i)
print("=== end of for-loop")

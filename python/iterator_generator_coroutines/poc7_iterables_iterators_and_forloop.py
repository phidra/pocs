#!/usr/bin/env python3
# coding: utf-8


"""
Cette POC montre que :
    - un ITERATOR est simplement quelque chose qui renvoie une valeur losqu'on lui appelle "next"
    - un ITERABLE est simplement quelque chose qui renvoie un iterator lorsqu'on lui appelle iter

Dit autrement, iter est simplement un moyen "d'initialiser" un iterator.

Pour bien montrer que l'ITERABLE et l'ITERATOR sont "décorrélés"...
...dans cette POC, tous les ITERABLES utilisent le même ITERATOR (partagé entre les itérables)

On voit que la for-loop est un wrapper autour du concept d'ITERABLE :
    - la for-loop attend un ITERABLE en entrée
    - elle appelle "iter" sur l'ITERABLE pour récupérer l'ITERATOR
    - elle exhauste les valeurs de l'ITERATOR
    - elle catche la StopIteration qui lui signale qu'il est temps d'arrêter d'itérer
"""


class SharedIterator():
    counter = 0
    limit = 15

    def __init__(self):
        print("instanciation of ITERATOR")

    def __next__(self):
        SharedIterator.counter += 1
        if SharedIterator.counter > 15:
            raise StopIteration()
        return SharedIterator.counter

global_iterator = SharedIterator()


class MySuperIterable():
    def __init__(self):
        print("instanciation of ITERABLE")

    def __iter__(self):
        return global_iterator


print("\n=== iterating using A")
shared_iterable_A = MySuperIterable()
shared_iterator_A = iter(shared_iterable_A)
print("next value A is ", next(shared_iterator_A))
print("next value A is ", next(shared_iterator_A))
print("next value A is ", next(shared_iterator_A))

print("\n=== iterating using B")
shared_iterable_B = MySuperIterable()
shared_iterator_B = iter(shared_iterable_B)
print("next value B is ", next(shared_iterator_B))
print("next value B is ", next(shared_iterator_B))
print("next value B is ", next(shared_iterator_B))

print("\n=== now mixing A and B")
print("next value A is ", next(shared_iterator_A))
print("next value B is ", next(shared_iterator_B))
print("next value A is ", next(shared_iterator_A))
print("next value B is ", next(shared_iterator_B))


print("\n=== seeing it in action in a for-loop")
print("=== now instanciating ITERABLE")
shared_iterable_C = MySuperIterable()
print("=== now using it in for-loop")
for i in shared_iterable_C:
    print(i)

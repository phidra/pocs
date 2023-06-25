#!/usr/bin/env python
# coding: utf-8

# OBJECTIF  = vérifier l'impact en terme de perfs des appels de fonctions/héritage de classe.
#
# PROTOCOLE = on compare le coût d'une opération (calculer un million de carrés) fait dans 4 cas :
#     - fonction DIRECT (la fonction calcule le carré)
#     - fonction NESTED (la fonction appelle une fonction qui appelle une fonction ... etc. qui calcule le carré)
#     - classe   DIRECT (on __call__ une classe qui calcule le carré directement)
#     - classe   NESTED (on __call__ une classe, qui hérite d'une classe, qui hérite d'une classe ... etc. qui calcule le carré)
#
# EXEMPLES DE RÉSULTATS :
#     DIRECT     took :  0:00:00.114981
#     NESTED     took :  0:00:00.815029
#     DIRECT CLS took :  0:00:00.205134
#     NESTED CLS took :  0:00:03.103794
#
# CONCLUSION DE LA POC : les appels de fonction imbriqués, ou les héritages à rallonge sont TRÈS COÛTEUX !


import random
import datetime

INPUT_SIZE = 1000 * 1000
INPUT = [random.randint(0, 9999) for _ in range(INPUT_SIZE)]


def direct_compute(x):
    return x * x


def nested_compute(x):
    return f1(x)
def f1(x):
    return f2(x)
def f2(x):
    return f3(x)
def f3(x):
    return f4(x)
def f4(x):
    return f5(x)
def f5(x):
    return f6(x)
def f6(x):
    return f7(x)
def f7(x):
    return f8(x)
def f8(x):
    return f9(x)
def f9(x):
    return f10(x)
def f10(x):
    return x*x


class Direct(object):
    def __call__(self, x):
        return x * x


class C10(object):
    def __call__(self, x): return x * x
class C9(C10):
    def __call__(self, x): return super(C9, self).__call__(x)
class C8(C9):
    def __call__(self, x): return super(C8, self).__call__(x)
class C7(C8):
    def __call__(self, x): return super(C7, self).__call__(x)
class C6(C7):
    def __call__(self, x): return super(C6, self).__call__(x)
class C5(C6):
    def __call__(self, x): return super(C5, self).__call__(x)
class C4(C5):
    def __call__(self, x): return super(C4, self).__call__(x)
class C3(C4):
    def __call__(self, x): return super(C3, self).__call__(x)
class C2(C3):
    def __call__(self, x): return super(C2, self).__call__(x)
class C1(C2):
    def __call__(self, x): return super(C1, self).__call__(x)
class Nested(C1):
    def __call__(self, x): return super(Nested, self).__call__(x)


def chronometrize(f, label):
    before = datetime.datetime.now()
    OUTPUT = [f(x) for x in INPUT]
    after = datetime.datetime.now()
    print "{:10} took : ".format(label), after-before
    return OUTPUT


# MAIN :
output1 = chronometrize(direct_compute, "DIRECT")
output2 = chronometrize(nested_compute, "NESTED")
output3 = chronometrize(Direct(), "DIRECT CLS")
output4 = chronometrize(Nested(), "NESTED CLS")
assert output1 == output2
assert output1 == output3
assert output1 == output4

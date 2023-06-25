#!/usr/bin/env python3.6
# coding: utf-8

"""
Résumé de la POC :
- une class est callable : le fait de call une class a pour effet d'en retourner une nouvelle instance
- lorsqu'on instancie une classe, __new__ et __init__ sont automatiquement (et successivement) appelées
- ces deux méthodes se voient passés les mêmes paramètres : ceux qu'on a passés au call de la class

Globalement, la doc est très claire :
    https://docs.python.org/3.6/reference/datamodel.html#object.__new__

Notamment, __init__ n'est appelée que si __new__ renvoie une instance de la classe.
Ça permet d'implémenter un singleton, à condition de s'assurer que __new__ renvoie une instance différente.
"""


def display(called_function, args, kwargs):
    """  displays the args and kwargs of the called_function """
    print(f"")
    print(f"{called_function} called with :")
    formatted_args = "|".join(list(repr(x) for x in args))
    formatted_kwargs = "|".join(list(f"{key}={value}" for key, value in kwargs.items()))
    print(f"    args  : {formatted_args}")
    print(f"    kwargs: {formatted_kwargs}")


# STEP 1 = montrer que __init__ et __new__ sont appelées successivement, et avec les mêmes arguments :
class Pouet:
    def __init__(self, val, *, kwval):
        display("Pouet::__init__", [val], {"kwval": kwval})

    def __new__(cls, *args, **kwargs):
        display("Pouet::__new__", args, kwargs)
        return super().__new__(cls)  # ici, le parent de Pouet (object) n'attend pas d'arguments !


p1 = Pouet(42, kwval="luke")
print("")
print("================================================================================")
p2 = Pouet(43, kwval="leia")


# STEP 2 = "mauvaise" implémentation d'un singleton
# une seule instance existe, mais chaque nouvelle tentative d'instanciation va la modifier
class WrongSingleton:
    _instance = None

    def __init__(self, age):
        self.age = age

    def __new__(cls, age):
        if WrongSingleton._instance is not None:
            return WrongSingleton._instance
        WrongSingleton._instance = super().__new__(cls)
        return WrongSingleton._instance

    def whatismyage(self, myname):
        print(f"{myname} has age {self.age}")

print("")
s1 = WrongSingleton(42)
s1.whatismyage("s1")
s2 = WrongSingleton(100)
assert(id(s1) == id(s2))  # OK : une seule instance existe
s1.whatismyage("s1")
s2.whatismyage("s2")
# assert(s1.age == 42)  # KO : cette assertion est fausse, car l'âge de s1 a été modifié par "l'instanciation" de s2


# STEP 3 = "bonne" implémentation d'un singleton
# comme __new__ renvoie une classe *différente*, init n'est pas appelé
class GoodSingleton:
    _instance = None

    class __RealClass:
        def __init__(self, age):
            self.age = age

        def whatismyage(self, myname):
            print(f"{myname} has age {self.age}")

    def __new__(cls, age):
        if GoodSingleton._instance is not None:
            return GoodSingleton._instance
        GoodSingleton._instance = GoodSingleton.__RealClass(age)
        return GoodSingleton._instance

    def __init__(self, *args, **kwargs):
        # cette méthode n'est jamais appelée, car GoodSingleton.__new__ ne renvoie pas une instance de GoodSingleton
        raise ValueError("this method should never been called")

print("")
s1 = GoodSingleton(42)
s1.whatismyage("s1")
s2 = GoodSingleton(100)
assert(id(s1) == id(s2))  # OK : une seule instance existe
s1.whatismyage("s1")
s2.whatismyage("s2")  # OK-ish : l'âge (100) passé à l'instanciation de s2 a été ignoré
assert(s1.age == 42)  # OK : l'âge de s1 n'a pas été modifié par "l'instanciation" de s2

#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# http://users.rcn.com/python/download/Descriptor.htm
# http://stackoverflow.com/questions/11949808/what-is-the-difference-between-a-function-an-unbound-method-and-a-bound-method
# https://wiki.python.org/moin/FromFunctionToMethod
# https://docs.python.org/2/reference/datamodel.html#implementing-descriptors
# https://docs.python.org/2/howto/descriptor.html
#
"""
Protocole descriptor
- ne s'applique que lorsque l'objet implémentant le protocole est un attribut d'une classe parent
- plus précisément, il apparaît dans le dictionnaire de classe du parent (dans le dictionnaire de classe d'un parent du parent)

object.__get__(self, instance, owner)
owner contient toujours la classe parente (celle qui CONTIENT l'objet implémentant le protocole)
note : comme par définition, le protocole descriptor ne s'applique que lorsqu'il y a un owner, il est toujours défini
instance contient l'instance par lequel on a accédé à l'attribut, ou None si on y a accédé via la classe.

Par définition, un objet implémentant le protocole descriptor est donc toujours un objet qui est "attaché" à un parent.

Un objet est un descriptor dès lors qu'il implémnete l'une des trois méthodes __get__ / __set__ / __delete.

Le comportement par défaut de ces méthodes (i.e. lorsqu'elles ne sont pas implémentées) est de maniupler le __dict__ de l'objet. :
    a.x    est équivalent à     type(a).__dict__['x'].__get__(a, type(a))
    A.x    est équivalent à     A.__dict__['x'].__get__(None, A)

Lorsqu'on implémente des méthodes, on modifie ce comportement par défaut.


"""
########################################################################################################################
print ""
print "============================================================================="
print "=== POC1 : une même définition de méthode peut donner naissance à des objets bien différents."
print "============================================================================="
print ""

class MyClass(object):
    def method(self):
        print "method"

myinstance = MyClass()

# Que sont les objets 

print "Affichage de la méthode en fonction de comment on la récupère : "
print ""

o1 = MyClass.__dict__['method']
print "MyClass.__dict__['method']   (id={}  /  type={:16})  = {}".format(id(o1), type(o1).__name__, o1)
#     MyClass.__dict__['method']   (id=3074959484  /  type=function        )  = <function method at 0xb748287c>

o2 = MyClass.method
print "MyClass.method               (id={}  /  type={:16})  = {}".format(id(o2), type(o2).__name__, o2)
#     MyClass.method               (id=3075218364  /  type=instancemethod  )  = <unbound method MyClass.method>

o3 = myinstance.method
print "myinstance.method            (id={}  /  type={:16})  = {}".format(id(o3), type(o3).__name__, o3)
#     myinstance.method            (id=3074801092  /  type=instancemethod  )  = <bound method MyClass.method of <__main__.MyClass object at 0xb728898c>>





print ""
print "============================================================================="
print "=== POC2 : on ne peut pas modifier directement une méthode d'une instance, car elle sera alors considérée comme simple fonction :"
print "============================================================================="
print ""
# Peut-on modifier la méthode d'une instance :
# Réponse, pas directement, car si on fait ça, le mécanisme d'appel de la méthode ne passe pas l'instance en premier argument
# D'ailleurs, on peut le voir en constatant que le type de myinstance.method n'est plus "bound method", mais est "fonction"

def replacement(instance):
    print "This is a replacement method."

print ">>> myinstance.method = replacement"
myinstance.method = replacement
print ""
print "Après remplacement simple, myinstance.method contient une simple fonction :"

o4 = myinstance.method
print "myinstance.method            (id={}  /  type={:16})  = {}".format(id(o4), type(o4).__name__, o4)


print ""
print "Par conséquent, on ne peut plus appeler la méthode  (qui n'en est pas vraiment une, vu que c 'est une simple fonction) :"
print ">>> myinstance.method()"
try:
    myinstance.method()
except TypeError, e:
    print "TypeError = ", e


print ""
print "============================================================================="
print "=== POC3 : pour modifier la méthode d'une instance, on récupère sa version bound via __get__ :"
print "============================================================================="
print ""

myinstance.method = replacement.__get__(myinstance, MyClass)
print myinstance.method
myinstance.method()


print ""
print "============================================================================="
print "=== POC4 : pour modifier la méthode d'une classe, on peut la définir normalement :"
print "============================================================================="
print ""

class Pouet(object):
    def method(self):
        print "initial method"

def func(obj):
    print "obj is {}".format(obj)

print "Pouet.method = ", Pouet.method
Pouet.method = func
print "Pouet.method = ", Pouet.method
Pouet.method = func.__get__(None, Pouet)
print "Pouet.method = ", Pouet.method

instance = Pouet()

instance.method()
Pouet.method(instance)
func(instance)

"""
J'avoue ne pascomprendre pourquoi je peux changer la méthode de la classe sans souci (POC4), mais pas de l'instance (POC2).
Plus exactement, je ne comprends pas pourquoi après le remplacement, myinstance.method renvoie une function (POC2) et Pouet.method renvoie une unbound (POC4)
"""

#NOTE : on est bien d'accord que l'objet renvoyé est différent selon la façon dont on y accède (cf. POC1).
# En effet, dans le cas 1 de la POC1, on récupère une fonction, alors que dans les cas 2 et 3, on récupère LE MEME OBJET instancemethod

# Ceci dit, je comprends bien pourquoi j'ai dû passer par l'appel direct à __get__ pour ce que je veux.
# En effet, comme je travaillais avec un décorateur, 


"""
Ce que je ne comprends pas, c'est que replacement, en tant qu'objet function (qui iplémente le protocole descriptor), dispose manifestement d'une méthode __get__.
Dans ce cas, pourquoi le remplacement direct ne marche pas ?
En effet, le mécanisme de résolution devrait marcher tout pareil...
Et pourquoi faut il appeler manuellement le mécanisme get ?

(peut-être parce que ce mécanisme d'attribution est fait au moment de définition de la méthode de classe, et pas au moment de la résoluton de nom ?)
(en pratique, a priori, non, car la POC4 fonctionne...
"""

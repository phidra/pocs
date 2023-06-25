#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# En complément de la POC sur factory_boy, cette POC montre des utilisations particulières :
#   - LazyFunction : pour qu'un attribut soit délégué à une fonction sans argument.
#   - LazyAttribute : pour qu'un attribut soit délégué à une fonction prenant l'objet créé en argument.
#   - Héritage : pour pouvoir spécialiser une factory.
#
# cf.
#   - http://factoryboy.readthedocs.io/en/latest/introduction.html#lazyfunction
#   - http://factoryboy.readthedocs.io/en/latest/introduction.html#lazyattribute
#   - http://factoryboy.readthedocs.io/en/latest/introduction.html#inheritance
#
########################################################################################################################

from factory import fuzzy, LazyFunction, LazyAttribute, Factory, lazy_attribute
import random
import sys ; sys.dont_write_bytecode = True

########################################################################################################################
# Modèle
########################################################################################################################

class Cake:
    def __init__(self, weight, salted, toppings, meal):
        self.weight = weight
        self.salted = salted
        self.toppings = toppings
        self.meal = meal
    def __repr__(self):
        return '''Cake(weight={}, salted={}, toppings={}, meal='{}')'''.format(
                self.weight, self.salted, self.toppings, self.meal)

########################################################################################################################
# Factories
########################################################################################################################

def _toppings_builder(obj):
    return ["Olive", "Cheese", "Chorizo"] if obj.salted else ["Chocolate", "Coconut"]
def _weight_builder():
    return random.choice(range(1, 16)) * 100


class CakeFactory(Factory):
    class Meta:
        model = Cake

    salted = fuzzy.FuzzyChoice([True, False])

    # LazyFunction
    weight = LazyFunction(_weight_builder)

    # LazyAttribute : définition 1 = en utilisant une fonction externe :
    toppings = LazyAttribute(_toppings_builder)

    # LazyAttribute : définition 2 = en décorant directement une fonction interne :
    @lazy_attribute
    def meal(self):
        """
        Choix du repas où on mange le cake :
            - si sucré, c'est petit-déjeuner
            - si salé et gros (plus d'1kg), c'est déjeuner
            - sinon (salé et petit), c'est dîner
        """
        if not self.salted:
            return "breakfast"
        else:
            return "lunch" if self.weight >= 1000 else "dinner"


class SmallDessertFactory(CakeFactory):
    u""" Spécialisation pour ne créer que des desserts, de petite taille, à manger à tous les repas."""
    salted = False
    weight = 150
    meal = fuzzy.FuzzyChoice(["breakfast", "dinner", "lunch"])

########################################################################################################################
# POC :
########################################################################################################################

print ""
print u"\n=== 10 Cakes générés par la CakeFactory :"
for i in range(10):
    print "\t", CakeFactory()

print ""
print u"\n=== 10 desserts générés par la factory :"
for i in range(10):
    print "\t", SmallDessertFactory()

########################################################################################################################
# Fin du fichier.
########################################################################################################################

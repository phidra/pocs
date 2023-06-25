#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# En complément de la POC sur factory_boy et la démo1, cette POC montre d'autres utilisations particulières :
#   - Params : pour définir un paramètre de la factory sans lien direct avec un champ du modèle.
#   - Traits : pour paramétrer le comportement de plusieurs champs à partir d'un flag booléen passé à la factory.
#   - Iterator : pour choisir une valeur dans un itérable.
#
# cf.
#   - http://factoryboy.readthedocs.io/en/latest/introduction.html#altering-a-factory-s-behaviour-parameters-and-traits
#   - http://factoryboy.readthedocs.io/en/latest/reference.html#iterator
#
# NOTE : nécessite une version récente de factory_boy (à froid : 2.5.2 insuffisant, 2.7.0 suffisant)
#
########################################################################################################################

from factory import fuzzy, LazyAttribute, Factory, Iterator, Trait
import datetime
import random
import sys ; sys.dont_write_bytecode = True

########################################################################################################################
# Modèle
########################################################################################################################

class MovieRental:
    def __init__(self, title, begin, end):
        self.title = title
        self.begin = begin
        self.end = end
    def __str__(self):
        return '{:15} : {} -> {}'.format(
                self.title, self.begin, self.end)

class ProductOrder:
    def __init__(self, name, customer, sending=None, reception=None):
        self.name = name
        self.customer = customer
        self.sending = sending
        self.reception = reception
    def __str__(self):
        product_str = '{:15} to {:15}'.format(self.name, self.customer)
        if self.sending:
            sending_str = '(shipped: {} -> {})'.format(self.sending, self.reception)
        else:
            sending_str = '(not shipped yet)'
        return product_str + ' ' + sending_str

########################################################################################################################
# Factories
########################################################################################################################

class MovieRentalFactory(Factory):
    class Meta:
        model = MovieRental
    title = Iterator(['Armaggedon', 'Mary Poppins', 'Star Wars', 'Harry Potter'])
    begin = fuzzy.FuzzyDate(start_date=datetime.date.today() - datetime.timedelta(10))
    end = LazyAttribute(lambda obj: obj.begin + datetime.timedelta(obj.duration))
    class Params:
        duration = 7 # default value

class ProductOrderFactory(Factory):
    class Meta:
        model = ProductOrder
    name = Iterator(['HardDrive', 'Keyboard', 'CPU', 'Motherboard'])
    customer = Iterator(['John Doe', 'Jane Dae', 'Bobby Lapointe', 'Billy Bob'])
    class Params:
        shipped = Trait(
            sending = fuzzy.FuzzyDate(start_date=datetime.date.today() - datetime.timedelta(10)),
            reception = LazyAttribute(lambda obj: obj.sending + datetime.timedelta(5))
                )


########################################################################################################################
# POC :
########################################################################################################################

print ""
print u"\n=== MovieRental générés par la Factory :"
for i in range(4):
    print "\t", MovieRentalFactory(duration=3*i)

print ""
print u"\n=== ProductOrder générés par la Factory :"
for i in range(4):
    print "\t", ProductOrderFactory(shipped=random.choice([True, False]))

########################################################################################################################
# Fin du fichier.
########################################################################################################################

#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Cette POC montre l'utilisation du module factory_boy :
#
#   pip install factory_boy
#
# cf. https://factoryboy.readthedocs.org/en/latest/
#
# DESCRIPTION DE LA POC :
#
#   L'objectif est de pouvoir créer avec la factory un objet du type suivant :
#
desired_object = {
    "feature1":
    {
        'active': True,
        'name': "This is the feature 1",
        'errors': []
    },
    "feature2":
    {
        'active': False,
        'name': "This is the feature 2",
        'errors': []
    },
    "feature3":
    {
        'active': True,
        'name': "This is the feature 3",
        'errors':
        [
            {
                "type": "dependancy_error",
                "message": "This feature has unsatisfied dependancies.",
                "extra":
                {
                    "unsatisfied" : ["feature1", "feature2"]
                }
            }
        ]
    }
}
#
########################################################################################################################
# NOTES D'UTILISATION DE FACTORY_BOY :
########################################################################################################################
#
# intérêt      = on simplifie la création d'objet en ne précisant que les paramètres qui diffèrent des valeurs par défaut.
# principe     = on définit une classe en modèle d'une factory, et les valeurs par défaut de ses attributs.
# le gros plus = on peut définir des valeurs par défaut "évoluées" :
#    Sequence = on définit des valeurs par défaut uniques par instance (ex: un pk)
#    LazyFunction = on délègue la valeur par défaut à une fonction (sans argument)
#    LazyAttribute = idem, à une fonction prenant l'instance en création en argument -> on peut faire dépendre certains attributs d'autres attributs
# astuces :
#    on peut créer plusieurs Factories différentes pour un même objet (ex: pour utiliser des attributs par défaut différents)
#    pour les objets avec ORM (ex: modèles django) il y a moyen de créer des objets avec ou sans enregistrement en base de données
#    il est possible de contrôler finement le compteur des séquences
#    on peut créer des listes d'objets en un coup
#
########################################################################################################################

import factory
import sys ; sys.dont_write_bytecode = True

########################################################################################################################
# Modèles utilisés par factory_boy comme templates (ils ne sont jamais utilisés directement).
########################################################################################################################

class Error:
    def __init__(self, type, message, extra):
        self.type = type
        self.message = message
        self.extra = extra
    def __repr__(self):
        return '''Error(type='{}', message='{}', extra={})'''.format(self.type, self.message, self.extra)

class Feature:
    def __init__(self, active, name, errors):
        self.active = active
        self.name = name
        self.errors = errors
    def __repr__(self):
        return '''Feature(active={}, name='{}', errors={})'''.format(
                self.active,
                self.name,
                self.errors)

########################################################################################################################
# Les factory correspondant aux modèles :
########################################################################################################################

class ErrorFactory(factory.Factory):
    class Meta:
        model = Error
    type = 'dependancy_error'
    message = 'This feature has unsatisfied dependancies.'
    extra = {'unsatisfied' : ['feature1', 'feature2']}

class FeatureFactory(factory.Factory):
    class Meta:
        model = Feature
    active = False
    name = factory.Sequence(lambda n: 'This is the feature {}'.format(n))
    errors = []
FeatureFactory.reset_sequence(1)

########################################################################################################################
# Les essais et la POC :
########################################################################################################################

print "\n=== Valeurs par défaut :"
print ErrorFactory()
print FeatureFactory()

print "\n=== Passage de valeurs :"
print ErrorFactory(type='special_type', message='Special message', extra={})
print FeatureFactory(active=True, errors='luke')

print "\n=== Dépendance définie manuellement :"
error1 = ErrorFactory(type='type1', message='msg1', extra={})
error2 = ErrorFactory(type='type2', message='msg2', extra={})
print error1
print error2
print FeatureFactory(active=True, errors=[error1, error2])

print "\n=== Dépendance batch :"
errors = ErrorFactory.build_batch(2, type='thetype', message='themsg', extra={})
print errors
print FeatureFactory(active=True, errors=errors)

# Il y a une astuce pour récupérer un dict à partir d'une factory :
#     mydict = factory.build(dict, FACTORY_CLASS=ErrorFactory, message="pouet")
# On peut donc créer des *AsDictFactory :
import functools
ErrorAsDictFactory = functools.partial(factory.build, dict, FACTORY_CLASS=ErrorFactory)
FeatureAsDictFactory = functools.partial(factory.build, dict, FACTORY_CLASS=FeatureFactory)

print "\n=== Récupération sous forme de dict :"
error1 = ErrorAsDictFactory(type='type1', message='msg1', extra={})
error2 = ErrorAsDictFactory(type='type2', message='msg2', extra={})
print error1
print error2
print FeatureAsDictFactory(active=True, errors=[error1, error2])


# On peut alors récupérer l'objet initialement voulu :
print "\n=== Premier essai d'objet final (les n° de features sont nuls, mais on s'en approche) :"
final_object_1 = {
        'feature1': FeatureAsDictFactory(active=True),
        'feature2': FeatureAsDictFactory(active=False),
        'feature3': FeatureAsDictFactory(active=True, errors=[ErrorAsDictFactory()]),
        }

import pprint
pprint.pprint(final_object_1, width=1)

# Comme les n°s de features sont différents, on passe plutôt par une méthode "as_dict" :
Feature.as_dict = lambda obj: { 'active': obj.active, 'name': obj.name, 'errors': obj.errors }
print "\n=== Deuxième essai d'objet final :"
FeatureFactory.reset_sequence(1)
final_object_2 = {
        'feature1': FeatureFactory(active=True).as_dict(),
        'feature2': FeatureFactory(active=False).as_dict(),
        'feature3': FeatureFactory(active=True, errors=[ErrorAsDictFactory()]).as_dict(),
        }

import pprint
pprint.pprint(final_object_2, width=1)

# On vérifie bien que l'objet obtenu est celui voulu :
assert(final_object_2 == desired_object)

########################################################################################################################
# Fin du fichier.
########################################################################################################################

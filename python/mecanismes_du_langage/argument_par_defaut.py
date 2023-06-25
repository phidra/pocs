#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC sur les arguments par défaut, et notamment ceux qui sont instanciés sur la ligne de définition de la fonction :
#       quand sont-ils instanciés ?
#       y en a-t-il un par appel ou un par fonction ?
#       peut-on y accéder en dehors de la fonction (introspection)
#       que se passe-t-il si la fonction le supprime ?
#
#
# CONCLUSIONS DE LA POC :
# 
# Lorsque l'argument par défaut est un objet déjà existant :
#     [POC 1] Lorsqu'on passe en argument par défaut un objet déjà existant, il n'est (bien évidemment) pas instancié à chaque fois.
#     [POC 1] Chaque appel de fonction créée, pendant la durée de vie de la fonction, une référence vers l'objet.
#             (sans doute celle correspondant au nom local de l'objet).
#     [POC 2] On peut le détruire à l'extérieur, la définition de fonction garde une référence, et l'objet n'est donc pas détruit
# 
# Lorsque l'argument par défaut est instancié pendant la définition de la fonction :
#     [POC 3] L'argument par défaut est instancié pendant la définition de la fonction.
#     [POC 3] Tous les appels utilisent le même argument par défaut (celui qui a été instancié pendant la définition).
#     [POC 4] On peut supprimer la variable référençant l'objet par défaut : il ne sera pas détruit.
#             (car la définition de la fonction en conserve une référence).
#     [POC 5] Si la fonction modifie l'argument par défaut, cette modification est GLOBALE à tous les appels.
#             (c'est le même objet qui est partagé pour tous les appels).
#     [POC 6] En utilisant inspect, il est possible de récupérer une référence sur l'argument par défaut.
#
########################################################################################################################

from sys import getrefcount
import inspect

########################################################################################################################
# POC 1 = Utiliser un objet déjà existant comme valeur par défaut n'en crée pas d'autres instances.
#         En revanche, la définition de la fonction incrémente le compteur de référence.
#         Et l'appel de la fonction incrémente temporairement (le temps de l'appel) le compteur de référence.
########################################################################################################################

class Dummy:
    u""" Classe vide ne servant qu'à fournir un objet."""
    pass

valeur_par_defaut = Dummy()

print ""
print "[POC1]   Avant définition de \"already_existing_object\", le getrefcount vaut : ", getrefcount(valeur_par_defaut)

def already_existing_object(mon_argument=valeur_par_defaut):
    u""" Fonction prenant toujours le même argument par défaut."""
    print "\t\t→ Inside \"already_existing_object\", le getrefcount vaut : ", getrefcount(mon_argument)

print "[POC1]   Après définition de \"already_existing_object\", le getrefcount vaut : ", getrefcount(valeur_par_defaut)

print "[POC1]   Trois appels successifs de \"already_existing_object\" :"
already_existing_object()
already_existing_object()
already_existing_object()
print "[POC1]   Après les appels, le getrefcount vaut : ", getrefcount(valeur_par_defaut)
print ""

########################################################################################################################
# POC 2 = On peut détruire la référence de l'objet à l'extérieur, ça ne change pas le fonctionnement.
#         En effet, la définition de la fonction conserve une référence vers l'objet (qui n'est donc pas détruit).
########################################################################################################################

print ""

print "[POC2]   Avant destruction de la référence \"valeur_par_defaut\", le getrefcount vaut : ", getrefcount(valeur_par_defaut)
del(valeur_par_defaut)
print "[POC2]   On vient de détruire la référence \"valeur_par_defaut\" (on ne peut donc plus compter directement les références)."
print "[POC2]   Mais l'appel à \"already_existing_object\" va nous fournir le décompte incrémenté de 1 :"
already_existing_object()
print "[POC2]   Après l'appel. On voit que del a détruit une référence."
print ""

########################################################################################################################
# POC 3 = Lorsqu'on instancie un objet dans la définition de la fonction, il n'est instancié qu'à la définition.
#         (et pas à chaque appel de fonction)
#         Par conséquent, c'est le MÊME objet qui est utilisé à chaque appel de fonction !
########################################################################################################################

class MyArg:
    u""" Classe affichant un print lors de l'instanciation."""
    object_counter = 0
    def __init__(self):
        MyArg.object_counter += 1
        print "[CONSTRUCTION D'UN NOUVEAU MyArg, IL Y EN A {} EN TOUT]".format(MyArg.object_counter)

################################################################################
print ""
print "[POC3]   Avant définition de la fonction \"instanciate_default_arg\"."
def instanciate_default_arg(mon_argument=MyArg()):
    u""" Fonction instanciant un objet comme valeur par défaut."""
    print "\t\t→ Inside \"instanciate_default_arg\", on utilise [{}], dont le getrefcount vaut {}".format(
                id(mon_argument), getrefcount(mon_argument))
print "[POC3]   Après définition de la fonction \"instanciate_default_arg\"."
print "[POC3]   On va faire 3 appels à \"instanciate_default_arg\" :"
instanciate_default_arg()
instanciate_default_arg()
instanciate_default_arg()
print "[POC3]   On voit que tous les appels utilisent le même objet."
print ""


########################################################################################################################
# POC 4 = À l'intérieur de la fonction, on peut le supprimer (del), ça ne supprimera que le binding de la variable locale.
#         L'objet référencé ne sera pas détruit, puisque la définition de la fonction en conserve une référence.
########################################################################################################################

print ""
print "[POC4]   Avant définition de la fonction \"del_default_arg\" :"
def del_default_arg(mon_argument=MyArg()):
    u""" Fonction supprimant l'objet par défaut instancié."""
    print "\t\t→ INSIDE \"del_default_arg\", on utilise [{}], dont le getrefcount vaut {}".format(
                id(mon_argument), getrefcount(mon_argument))
    print "\t\t→ Avant sa suppression..."
    del(mon_argument)
    print "\t\t→ Après sa suppression..."
print "[POC4]   Après définition de la fonction \"del_default_arg\" :"
print "[POC4]   On va faire 3 appels à \"del_default_arg\" :"
del_default_arg()
del_default_arg()
del_default_arg()
print "[POC4]   On voit que la suppression ne détruit pas l'objet."
print ""


########################################################################################################################
# POC 5 = Toute modification effectuée sur l'argument par défaut est GLOBALE à tous les appels de fonction.
########################################################################################################################

print ""
print "[POC5]   Avant définition de la fonction \"modify_default_arg\" :"

def modify_default_arg(mon_argument=list()):
    print "\t\t→ INSIDE \"modify_default_arg\", on utilise la liste [{}], dont le getrefcount vaut {}".format(
                id(mon_argument), getrefcount(mon_argument))
    print "\t\t→ La liste contient {} élément(s), on va lui en ajouter dix de plus.".format(len(mon_argument))
    mon_argument.extend([x for x in range(10)])

print "[POC5]   Après définition de la fonction \"modify_default_arg\"."
print "[POC5]   Trois appels successifs à \"modify_default_arg\" :"
modify_default_arg()
modify_default_arg()
modify_default_arg()
print "[POC5]   Après les trois appels, on constate que le MÊME objet est bien modifié à chaque appel."
print ""


########################################################################################################################
# POC 6 = En rusant un peu, on peu accéder à un argument directement instancié pendant la définition d'une fonction.
########################################################################################################################

class MyVariableArg:
    u""" Classe pouvant être modifiée."""
    def __init__(self):
        self.answer_to_big_question = 42


print ""
print "[POC6]   Avant définition de la fonction \"external_modify_default_arg\" :"

def external_modify_default_arg(mon_argument=MyVariableArg()):
    print "\t\t→ INSIDE \"external_modify_default_arg\", on utilise [{}], dont le getrefcount vaut {}".format(
                id(mon_argument), getrefcount(mon_argument))
    print "\t\t→ Quelle est la réponse à la question : ", mon_argument.answer_to_big_question

print "[POC6]   Après définition de la fonction \"external_modify_default_arg\" :"
print "[POC6]   On va faire 2 appels à la fonction :"
external_modify_default_arg()
external_modify_default_arg()
print "[POC6]   Maintenant, en utilisant le module \"inspect\" pour y accéder, on va modifier l'argument par défaut :"
(args, varargs, keywords, default) = inspect.getargspec(external_modify_default_arg)
the_default_argument = default[0]
the_default_argument.answer_to_big_question = 101
print "[POC6]   On fait 2 nouveaux appels à la fonction, et on constate que l'argument par défaut a changé !"
external_modify_default_arg()
external_modify_default_arg()
print ""



########################################################################################################################
# Fin du fichier.
########################################################################################################################


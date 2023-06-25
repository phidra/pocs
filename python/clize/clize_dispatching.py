#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Démonstration du dispatch des commandes clizes vers diverses fonctions (dont une définie dans un module externe).
########################################################################################################################

import clize
import module_externe
 
@clize.clize()
def premiere_sous_commande(arg1, arg2, option1 = False, option2 = True, option3 = "patate", option4 = 42., *autres_args):
    u"""
        Je suis le début de l'aide de la docstring.

        option1: une option qui ne sert à rien.

        option2: une autre option qui ne sert à rien (en fait, aucune option ne sert, vu qu'on est dans une poc).

        arg2: le deuxième argument... bizarre, n'est-ce pas ?

        Voilà, c'est la fin de l'aide de la docstring.
    """
    print "Appel de la sous-commande n°1"
    print "Option 1 = ", option1
    print "Option 2 = ", option2
    print "Option 3 = ", option3
    print "Option 4 = ", option4
    print "arg 1 = ", arg1
    print "arg 2 = ", arg2
    print "autres args :"
    for arg in autres_args:
        print "\t- ", arg

@clize.clize()
def deuxieme_sous_commande():
    """ Je ne prends pas d'argument."""
    print "Appel de la sous-commande n°2"

########################################################################################################################
# MAIN = tester clize
########################################################################################################################

if __name__ == '__main__':
    clize.run([premiere_sous_commande, deuxieme_sous_commande, module_externe.troisieme_sous_commande], description = u"dispatcher vers trois fonctions", footnotes = u"rien de plus à ajouter")

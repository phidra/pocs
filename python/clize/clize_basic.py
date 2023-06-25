#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
########################################################################################################################

import clize
 
@clize.clize()
def mon_interface(arg1, arg2, option1 = False, option2 = True, option3 = "patate", option4 = 42., *autres_args):

    print "Option 1 = ", option1
    print "Option 2 = ", option2
    print "Option 3 = ", option3
    print "Option 4 = ", option4

    print "arg 1 = ", arg1
    print "arg 2 = ", arg2

    print "autres args :"
    for arg in autres_args:
        print "\t- ", arg

########################################################################################################################
# MAIN = utiliser ma fonction en tant qu'interface de mon programme :
########################################################################################################################

if __name__ == '__main__':
    clize.run(mon_interface)

"""
NOTES :
    ------------------------------------------------------------
    précisions terminologiques :
        argument de la fonction python  = ici, les arguments de la fonction "ma_fonction_de_test"
        argument de la CLI              = ce qu'on passe à la CLI, qui ne commence pas par "--" (ou "-" pour les versions courtes)
        option de la CLI                = ce qu'on passe à la CLI, qui commence par "--" (ou "-" pour les versions courtes)
        option sans argument de la CLI  = option qui est auto-porteuse (ex: --my-option-without_arg)
        option avec argument de la CLI  = option qui nécessite une valeur (ex: --my-option-with_arg=42)
    ------------------------------------------------------------
    les arguments de la fonction python qui n'ont pas de valeur par défaut seront des arguments de la CLI
        ex: ./my_CLI_test.py arg1 arg2
    les arguments de la fonction python qui ont une valeur par défaut booléenne seront des options sans argument de la CLI
        ex: ./my_CLI_test.py --option-without-argument
    les autres arguments de la fonction python qui ont une valeur par défaut sont des options avec argument
        ex: ./my_CLI_test.py --option-with-argument=42 --other-option="patate"
    les arguments des options sont typés en int, string ou float (qu'on indique grâce à la valeur par défaut de l'argument python)
    ------------------------------------------------------------
    clize remplit tout seul ce que renvoie --help
    notamment, il indique la façon d'utiliser la commande (usage)
    et il remplit automatiquement la description de la commande avec 3 données :
        la docstring est utilisée pour décrire ce que fait la fonction (utiliser une docstring unicode si on veut y mettre des accents)
        les arguments de la CLI (les arguments non-nommés sont utilisés aussi, et suffixés dans le --help par "...")
        les options de la CLI
    on peut documenter automatiquement plus précisément les arguments/options, avec la syntaxe suivante à placer dans la docstring :

        ARGUMENT: la description de l'argument

        OPTION: la description de l'option

    Attention : il est bien important de sauter des lignes entre chaque description d'option/argument.
    ------------------------------------------------------------
    la syntaxe **autres_options (qui aurait peut-être récupéré les options non-documentées dans un dict ?) n'est pas reconnue.
    ------------------------------------------------------------
    TRICKY TRICKS :
        rendre obligatoire la présence d'au moins un argument "additionel" :
            habituellement, en python les arguments additionels (ici, *autres_args) sont optionnels
            ce n'est pas forcément le comportement voulu (ex: si je veux traiter tous les fichiers QIF passés en argument)
            pour forcer l'utilisateur à passer au moins un argument "additionel", on doit paramétrer le décorateur clize :
                clize(require_excess = True)
        ajouter une "version courte" pour une option :
            on peut avoir l'option "--this-is-MY-option", vouloir permettre à l'utilisateur de l'appeler avec "-M"
            pour cela, il faut paramétrer le décorateur clize pour lui passer un dict contenant les alias, pointant vers une liste :
                clize(alias = { "this_is_MY_option":("-M",) })
        mapper une option spéciale sur une autre fonction :
            ex: l'option "--version" (aliasée par "-v") mappée sur une fonction show_version_and_exit :
            voir l'exemple "echo.py" distribué avec le package clize
            (en gros, on paramètre le décorateur clize avec "extra", et on utilise la fonction make_flags pour le mapping)
        dispatcher les actions accessibles sur différentes fonctions :
            en passant comme argument de la fonction clize.run un itérable contenant des fonctions, on créé un dispatcher)
            toutes les fonctions dispatchées doivent avoir été décorées par clize.clize
            help : on peut faire ./dispatcher.py mycommand --help pour récupérer la doc de la commande "mycommand"
            (pour documenter directement le dispatcher, on peut utiliser les kwargs description et footnotes de la méthode clize.run)
            les fonctions dispatchées peuvent être dans un autre module que le main !
        message d'erreur "TypeError: list() takes no arguments (2 given)" :
            ça arrive quand on a précisé une fonction à utiliser comme CLI, mais qu'elle n'est pas décorée par le décorateur clize
    ------------------------------------------------------------
    QUESTION :
        comment indique-t-on qu'une option n'a pas de valeur par défaut (i.e. elle est obligatoire) ?
"""

########################################################################################################################
# Fin du fichier.
########################################################################################################################



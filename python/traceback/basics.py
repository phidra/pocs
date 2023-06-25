#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC d'utilisation du module traceback pour stocker dans une string le contenu d'une exception (ex: pour la logger).
# NOTE : cf. la doc du module
########################################################################################################################

import traceback
import datetime

########################################################################################################################
# On crée une chaîne de fonction pour "remplir" un peu la traceback :
########################################################################################################################

def f0():
    raise ValueError("Ceci n'est pas une exception !")
def f1():
    return f0()
def f2():
    return f1()
def f3():
    return f2()
def f4():
    return f3()
def f5():
    return f4()

########################################################################################################################
# Le main est un simple appel à la fonction de plus haut-niveau, encapsulé dans un try/catch :
########################################################################################################################

try:
    f5()
except Exception, e:

    msg = "\n"
    msg += datetime.datetime.now().strftime("%Y-%m-%d-%Hh%Mm%Ss")
    msg += "\n===== EXCEPTION : {}".format(e.__class__.__name__)
    msg += "\n===== TEXT = {}".format(str(e))

    trace_string = traceback.format_exc()  # On peut aussi afficher/.logger directement, cf. print_exc
    msg += "\n===== TYPE(trace_string) = {}".format(type(trace_string).__name__)
    msg += "\n===== Valeur de la trace_string : "
    msg += "\n"
    msg += trace_string

    # On peut alors afficher la stacktrace :
    print msg

    # Ou bien l'enregistrer simplement dans un fichier :
    with open("/tmp/traceback_POC.log", "a") as f:
        f.write(msg)


########################################################################################################################
# Fin du fichier
########################################################################################################################

#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# QUESTION = comment connaître la consommation mémoire du programme ?
# RÉPONSE  = pas possible de faire ça directement en python, mais on peut appeler des outils externes pour le faire.
#
########################################################################################################################

import os
import time
import subprocess


# Fonction analysant la mémoire consommée :
def memory_consumption():
    command = "ps u -p " + str(os.getpid()) + " | awk '{sum=sum+$6}; END {print sum}'"
    p = subprocess.Popen(command, shell = True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = p.communicate()
    if p.returncode != 0:
        raise ValueError("non-zero returncode ({rc}), with stdout=[{out}] and stderr=[{err}]".format(rc=p.returncode, out=out, err=err))
    return int(out.rstrip("\n"))

     
# Boucle consommant de la mémoire à chaque itération :
list_of_list = []
while True:
    time.sleep(1)
    list_of_list.append(range(1000 * 1000))
    print memory_consumption(), "kib"

########################################################################################################################
# Fin du fichier.
########################################################################################################################


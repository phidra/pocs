#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Démonstration de la façon d'utiliser subprocess.
########################################################################################################################

import subprocess
import os

command1 = "ps u -p " + str(os.getpid())
command2 = ["ps", "u", "-p", str(os.getpid())]

# Démonstration en mode Shell (dangereux, mais plus pratique) :
p1 = subprocess.Popen(command1, shell = True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out1, err1 = p1.communicate()
if p1.returncode != 0:
    raise ValueError("non-zero returncode ({rc}), with stdout=[{out}] and stderr=[{err}]".format(rc=p1.returncode, out=out, err=err))

# Démonstration sans mode Shell :
p2 = subprocess.Popen(command2, shell = False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
out2, err2 = p2.communicate()
if p2.returncode != 0:
    raise ValueError("non-zero returncode ({rc}), with stdout=[{out}] and stderr=[{err}]".format(rc=p2.returncode, out=out, err=err))


print "SORTIE 1 ="
print out1

print "SORTIE 2 ="
print out2


########################################################################################################################
# Fin du fichier.
########################################################################################################################


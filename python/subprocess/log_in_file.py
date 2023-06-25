#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# Enregistrement de la sortie du subprocess dans un fichier :
########################################################################################################################

import subprocess
import os

LOGFILE_PATH = "/tmp/PYPOC_log_in_file.log"

command = ["ps", "u", "-p", str(os.getpid())]

with open(LOGFILE_PATH, "w") as f:
    p = subprocess.Popen(command, shell = False, stdout=f, stderr=f)
    out, err = p.communicate()
    if p.returncode != 0:
        raise ValueError("non-zero returncode ({rc}), with stdout=[{out}] and stderr=[{err}]".format(rc=p.returncode, out=out, err=err))

########################################################################################################################
# Fin du fichier.
########################################################################################################################


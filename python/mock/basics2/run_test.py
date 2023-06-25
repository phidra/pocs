#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Test de la fonction de traitement (process) ; pour supprimer le caractère aléatoire de "randint", on la mocke.
#
# Voir les explications ci-dessous au fil de l'exécution.
#
########################################################################################################################


import sys ; sys.dont_write_bytecode = True
import mock
import unittest

import run_poc
# MA COMPRÉHENSION DE CE QUI SE PASSE À L'EXÉCUTION DE LA LIGNE CI-DESSUS :
# L'importaton de run_poc entraîne l'exécution de son code, et donc l'importation de intgetter.
# L'importation de intgetter créée l'objet module à partir du fichier "intgetter.py".
# De plus, dans l'espace de nom de run_poc, un nom local "intgetter" est créé, qui pointe vers cet objet module.

class TestProcess(unittest.TestCase):
    """
    Test unitaire de la fonction de process.
    """

    def test_with_zero(self):
        """process doit renvoyer 42 lorsqu'elle reçoit 0 """

        run_poc.intgetter.randint = mock.Mock(return_value=0)
        # MA COMPRÉHENSION DE CE QUI SE PASSE À L'EXÉCUTION DE LA LIGNE CI-DESSUS :
        # Cette ligne récupère la variable de nom "run_poc" dans l'espace global du test, qui pointe vers le module "run_poc" importé.
        # Ce module run_poc a son propre espace de nom, qui contient notamment la variable de nom "intgetter".
        # Ainsi, run_poc.intgetter pointe vers l'objet module "intgetter" importé plus haut.
        # Au final, que fait cette ligne de code ?
        # Elle remplace dans le module "intgetter", la variable de nom "randint" (i.e. la fonction randint) par un mock.

        self.assertEqual(run_poc.process(), 42)
        # MA COMPRÉHENSION DE CE QUI SE PASSE À L'EXÉCUTION DE LA LIGNE CI-DESSUS :
        # On récupère la variable "run_poc" dans l'espace global du test, qui pointe vers l'objet module run_poc importé.
        # On exécute la méthode "process" de cet objet module run_poc.
        # Cette méthode process du module run_poc exécute la ligne de code suivante :
        #     myint = intgetter.randint()
        # C'est à dire qu'on récupère (dans l'espace de nom de run_poc) la variable intgetter, donc le module intgetter.
        # Puis on récupère dans intgetter la variable de nom "randint", et on l'exécute.
        # Or, il se trouve que cette variable "randint" dans le module intgetter a été mockée juste avant !
        # On se retrouve donc bien avec une valeur mockée, ce qui permet in fine la répétabilité du test.


    def test_with_nonzero(self):
        """process doit renvoyer le double de l'entrée lorsqu'elle reçoit autre chose que 0 """
        for i in range(1,10):
            run_poc.intgetter.randint = mock.Mock(return_value=i)
            self.assertEqual(run_poc.process(), 2*i)

unittest.main()

########################################################################################################################
# MA COMPRÉHENSION DE CE QUI SE PASSE :
########################################################################################################################


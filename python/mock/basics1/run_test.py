#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Lancement du test de la fonction de traitement (process).
# Pour supprimer le caractère aléatoire de "randint", on la mocke.
#
########################################################################################################################


import sys ; sys.dont_write_bytecode = True
import mock
import unittest

class TestProcess(unittest.TestCase):
    """
    Test unitaire de la fonction de process.
    """

    def test_with_zero(self):
        """process doit renvoyer 42 lorsqu'elle reçoit 0 """
        import run_poc
        run_poc.randint = mock.Mock(return_value=0)
        self.assertEqual(run_poc.process(), 42)

    def test_with_nonzero(self):
        """process doit renvoyer le double de l'entrée lorsqu'elle reçoit autre chose que 0 """
        for i in range(1,6):
            import run_poc
            run_poc.randint = mock.Mock(return_value=i)
            self.assertEqual(run_poc.process(), 2*i)

unittest.main()

########################################################################################################################
# Fin du fichier.
########################################################################################################################


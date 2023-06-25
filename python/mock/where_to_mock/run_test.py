#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
# POC = implémentation de l'exemple donné ici :
#
#    https://docs.python.org/3/library/unittest.mock.html#where-to-patch
#
# cf. les explications printées ci-dessous pour comprendre la POC.
#
########################################################################################################################


import sys ; sys.dont_write_bytecode = True
import mock
import unittest
import b1
import b2

class TestOfb1(unittest.TestCase):

    @mock.patch("b1.SomeClass")
    def test_mocking_b1SomeClass(self, mocked_class):

        b1.some_function(1)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 1)
        mocked_class.reset_mock()

        b1.some_function(3)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 3)

    @mock.patch("a.SomeClass")
    def test_mocking_aSomeClass(self, mocked_class):

        b1.some_function(1)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 1)
        mocked_class.reset_mock()

        b1.some_function(3)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 3)

class TestOfb2(unittest.TestCase):

    @mock.patch("b2.SomeClass")
    def test_mocking_b2SomeClass(self, mocked_class):

        b2.some_function(1)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 1)
        mocked_class.reset_mock()

        b2.some_function(3)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 3)

    @mock.patch("a.SomeClass")
    def test_mocking_aSomeClass(self, mocked_class):

        b2.some_function(1)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 1)
        mocked_class.reset_mock()

        b2.some_function(3)
        self.assertTrue(mocked_class.called)
        self.assertEqual(mocked_class.call_count, 3)


print """
=========================================
=== EXPLICATIONS DES TESTS CI-DESSOUS ===
=========================================
OBJECTIF DE LA POC : montrer qu'il faut mocker le nom de variable réellement utilisé par le module testé.

Dans tous les tests, on vérifie que l'appelle à bX.some_function(N) instancie N fois a.SomeClass.
Pour cela, on mocke SomeClass (question centrale de la POC : où mocker SomeClass ?), et on compte ses appels.

On fait 4 tests au total, selon qu'on teste b1 ou b2, et en mockant a.SomeClass ou b.SomeClass :
    testing b1.some_function  +  mocking a.SomeClass    -> FAIL : on n'a pas mocké le nom "SomeClass" utilisé par b1
    testing b1.some_function  +  mocking b1.SomeClass   -> OK
    testing b2.some_function  +  mocking a.SomeClass    -> OK
    testing b2.some_function  +  mocking b2.SomeClass   -> ERROR : b2 n'a pas de nom "SomeClass" à mocker

Cette POC illustre qu'il faut bien mocker le nom "SomeClass" utilisé par bX :

    pour b1, c'est b1.SomeClass qui est utilisée (donc que l'on doit mocker), car la ligne d'import est :

        from a import SomeClass

    pour b2, c'est a.SomeClass qui est utilisée (donc que l'on doit mocker), car la ligne d'import est :

        import a

À NOTER : Tout ceci est vrai, car bX.some_function utilise directement SomeClass (en l'instanciant).
          Mais si, au lieu de vouloir instancier SomeClass, on l'instanciait puis utilisait une de ses méthodes,
          alors peu importe si on mockait a.SomeClass.method ou b1.SomeClass.method, vu que dans les deux cas
          a.SomeClass et b1.SomeClass représentent le MÊME objet.
=========================================
========= FIN DES EXPLICATIONS ==========
=========================================
"""

unittest.main()

########################################################################################################################
# Fin du fichier.
########################################################################################################################


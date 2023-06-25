#!/usr/bin/env python
# coding: utf-8
########################################################################################################################
#
# Cette POC montre l'utilisation du module mock (unittest.mock en python3).
# (intégré à la lib standard en 3.3, backporté via pip pour les versions à partir de 2.6)
#
# Installation pour python < 3.3 :
#
#   pip install mock
#
# cf. https://docs.python.org/3/library/unittest.mock.html
#
########################################################################################################################
# DESCRIPTION DE LA POC :
########################################################################################################################
#
# En fonctionnement normal, la POC est consituée de deux modules :
#     - un colorserver (qui doit être lancé de façon externe) répondant à chaque requête par une couleur
#     - le présent module, qui interroge le serveur de couleur, et l'affiche à l'écran
#
# L'intérêt de la POC est d'utiliser unittest.mock pour tester la fonction d'affichage "skydisplay".
#
# Exécution de la POC en fonctionnement normal :
#     - lancer le colorserver dans un terminal externe
#     - exécuter le présent module
#     - les couleurs renvoyées par le serveur externe s'affichent à l'écran
#
# Exécution de la POC en mode "test" :
#     - NE PAS LANCER de colorserver (pas besoin, c'est justement l'intérêt de la POC de pouvoir s'en passer)
#     - exécuter le présent module avec l'argument "test"
#     - les tests unitaires (implémentés dans la classe unittest qui va bien) se lancent
#
########################################################################################################################

import mock
import random
import time
import requests
import sys
import unittest

COLORSERVER = "http://localhost:8888/"
COLORSERVER_SCRIPT = "colorserver.py"
ALLOWED_COLORS = ('white', 'black', 'green', 'red')


def ask_for_skycolor():
    """
    Fonction requêtant une couleur auprès du colorserver.
    C'est cette fonction qu'on va mocker lorsqu'on va tester skydisplay.
    """
    response = requests.get(COLORSERVER)
    return response.content.strip("\n")


def skydisplay():
    """
    Fonction requêtant la couleur du ciel, et l'affichant.
    C'est la fonction à tester unitairement.
    Doit indiquer une "unexpected color" si la couleur n'est pas dans les valeurs autorisées.
    Doit lever une ValueError si aucune couleur n'est renvoyée.
    """
    skycolor = ask_for_skycolor()
    if not skycolor:
        raise ValueError("no skycolor")
    elif skycolor not in ALLOWED_COLORS:
        return "The sky is of unexpected color !"
    else:
        return "The sky is ... {}".format(skycolor)


# MAIN 
def run_normally():
    """
    Exécution normale = on requête et affiche 10 fois la couleur du ciel.
    (on attend entre 0 et 1 secondes entre chaque)
    """
    try:
        for i in range(10):
            print skydisplay()
            time.sleep(random.randint(0,10) / 10.0)
    except requests.exceptions.ConnectionError:
        print ""
        print "Connection to colorserver refused !"
        print "Please launch colorserver externally with :"
        print ""
        import os.path
        dirname = os.path.dirname(sys.argv[0])
        print "\t{}".format(os.path.join(dirname, COLORSERVER_SCRIPT))


class TestSkydisplay(unittest.TestCase):
    """
    Test unitaire de la fonction skydisplay.
    """

    def test_valid(self):
        """
        Vérifie que skydisplay se comporte bien comme attendu sur les couleurs autorisées.
        """
        for color in ALLOWED_COLORS:
            global ask_for_skycolor
            ask_for_skycolor = mock.Mock(return_value=color)
            self.assertEqual(skydisplay(), "The sky is ... {}".format(color))
            ask_for_skycolor.assert_called_once_with()


    def test_invalid(self):
        """
        Vérifie que skydisplay se comporte bien comme attendu sur des couleurs inattendues.
        """
        invalid_colors = ("blue", "yellow")

        for invalid_color in invalid_colors:
            # (on vérifie que la couleur est bien invalide, sans quoi le test n'a pas d'intérêt)
            self.assertNotIn(invalid_color, ALLOWED_COLORS)
            global ask_for_skycolor
            ask_for_skycolor = mock.Mock(return_value=invalid_color)
            self.assertEqual(skydisplay(), "The sky is of unexpected color !")
            ask_for_skycolor.assert_called_once_with()

    def test_empty(self):
        """
        Vérifie que skydisplay se comporte bien comme attendu sur un retour vierge.
        """
        global ask_for_skycolor
        ask_for_skycolor = mock.Mock(return_value='')
        with self.assertRaises(ValueError):
            skydisplay()
            ask_for_skycolor.assert_called_once_with()


if not "test" in sys.argv:
    print "NOTE : run  '{} test'  to launch tests...".format(sys.argv[0])
    print ""
    run_normally()
else:
    sys.argv = [sys.argv[0]] # On supprime l'argument pour ne pas interférer avec unittest
    unittest.main()

########################################################################################################################
# Fin du fichier.
########################################################################################################################

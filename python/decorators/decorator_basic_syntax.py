#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Cette pypoc montre une syntaxe de base directement utilisable pour décorer une fonction.
#     http://sametmax.com/comprendre-les-decorateurs-python-pas-a-pas-partie-1/
#     http://sametmax.com/comprendre-les-decorateur-python-pas-a-pas-partie-2/
#
# En résumé :
#     syntaxe 1 = décorateur figé d'une fonction avec arguments
#     syntaxe 2 = décorateur paramétrable d'une fonction avec paramètres
#
########################################################################################################################


########################################################################################################################
# SYNTAXE 1 = décorateur figé d'une fonction avec arguments
########################################################################################################################

# Fonction simple (non-décorée)
def pouet(qui, quoi):
    print "    Pouet {quoi}, mon cher {qui} !".format(qui=qui, quoi=quoi)

# Le décorateur, qui affiche les arguments :
def mydecorator(f):
    def _wrapper(*args, **kwargs):
        kwarglist = ["{}->{}".format(key, value) for key, value in kwargs.iteritems()]
        print "    - arglist   = ", " ".join(args)
        print "    - kwarglist = ", " ".join(kwarglist)
        return f(*args, **kwargs)
    return _wrapper

# Fonction décorée :
@mydecorator
def pouet2(qui, quoi):
    print "    Pouet {quoi}, mon cher {qui} !".format(qui=qui, quoi=quoi)


# MAIN = appeler les deux fonctions :
print ""
print u"=== Fonction simple (non-décorée) :"
pouet("Watson", quoi="moufette")
print ""
print u"=== Fonction décorée :"
pouet2("Watson", quoi="moufette")


########################################################################################################################
# SYNTAXE 2 = décorateur paramétrable d'une fonction avec paramètres
########################################################################################################################

# Le décorateur paramétrable, qui affiche les arguments :
def decoratorfactory(showargs, showkwargs):
    def _decorator(f):
        def _wrapper(*args, **kwargs):
            kwarglist = ["{}->{}".format(key, value) for key, value in kwargs.iteritems()]
            if showargs:
                print "    - arglist   = ", " ".join(args)
            if showkwargs:
                print "    - kwarglist = ", " ".join(kwarglist)
            return f(*args, **kwargs)
        return _wrapper
    return _decorator

# Fonction décorée ne montrant que les args (pas les kwargs) :
@decoratorfactory(showargs=True, showkwargs=False)
def pouet3(qui, quoi):
    print "    Pouet {quoi}, mon cher {qui} !".format(qui=qui, quoi=quoi)

# Fonction décorée ne montrant que les kwargs (pas les args) :
@decoratorfactory(showargs=False, showkwargs=True)
def pouet4(qui, quoi):
    print "    Pouet {quoi}, mon cher {qui} !".format(qui=qui, quoi=quoi)


# MAIN = appeler les deux fonctions :
print ""
print u"=== Fonction décorée, ne montrant que les args (pas les kwargs) :"
pouet3("Watson", quoi="moufette")
print ""
print u"=== Fonction décorée, ne montrant que les kwargs (pas les args) :"
pouet4("Watson", quoi="moufette")



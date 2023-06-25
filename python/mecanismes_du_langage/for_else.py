#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Démonstration de la clause "else" d'une boucle for (on passe toujours dedans, sauf si on est sorti avec un break).
#
########################################################################################################################


skywalkers = ["Luke", "Leia", "Anakin"]
jedis = ["Yoda", "Luke", "Obi-wan"]

gentils = ["Luke", "Leia", "Obi-wan", "Yoda"]
mechants = ["Palpatine", "Anakin"]



# Une boucle qui n'appelle pas break PASSE dans le else :
print "Tous les jedis sont-ils gentils ?"
for jedi in jedis:
    if jedi in mechants:
        break
    else:
        print "\t", jedi, "est un gentil jedi."
else:
    print "Tous les jedis sont gentils !"


# Une boucle qui appelle break NE PASSE PAS dans le else :
print ""
print "Tous les skywalkers sont-ils gentils ?"
for skywalker in skywalkers:
    if skywalker in mechants:
        break
    else:
        print "\t", skywalker, "est un gentil skywalker"
else:
    # Cet affichage N'APPARAITRA PAS :
    print "Tous les skywalkers sont gentils !"


########################################################################################################################
# Fin du fichier.
########################################################################################################################

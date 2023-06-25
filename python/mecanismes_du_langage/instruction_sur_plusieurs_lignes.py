#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# POC sur le fait de splitter une instruction sur plusieurs lignes.
#
# TL;DR = splitter une instruction au milieu d'un couple (parenthèse/accolades/crochets) + ne pas se soucier de l'indentation.
#
#
# En un peu plus détaillé :
#   - par défaut, splitter une instruction sur plusieurs lignes ne fonctionne pas
#   - on peut splitter une instruction sur plusieurs lignes si on ne ferme pas un couple de parenthèses/crochets/accolades.
#   - dans ce cas, PEU IMPORTE l'indentation des morceaux de lignes (tant qu'elle est légale)
#   - en alternative moins intéressante, on peut aussi mettre un antislash comme dernier caractère de la ligne
#   - REMARQUE : comme les appels de fonction utilisent des parenthèse, on peut les splitter comme on l'entend !
#
# PEP 8 = http://legacy.python.org/dev/peps/pep-0008/
#
########################################################################################################################

########################################################################################################################
# PRINCIPE 1 = par défaut, séparer une instruction sur plusieurs lignes ne fonctionne pas :
########################################################################################################################

print """Lorsqu'exécuté, le code suivant lève une SyntaxError :

a = 1 + 2 + 3 + 4 + 5 + 6 + 7

b = 1 + 2 + 3 +
4 + 5 + 6 + 7

c = 1 + 2 + 3 +
    4 + 5 + 6 + 7
d = 1 + 2 + 3 + \
        4 + 5 + 6 + 7
"""

########################################################################################################################
# PRINCIPE 2 = on peut séparer une instruction en mettant un antislash comme dernier caractère de la ligne :
#              (dans ce cas, peu importe l'indentation de la ligne suivante, tant qu'elle est légale)
########################################################################################################################
a = 1 + 2 + 3 + 4 + 5 + 6 + 7

b = 1 + 2 + 3 + \
4 + 5 + 6 + 7

c = 1 + 2 + 3 + \
    4 + 5 + 6 + 7

d = 1 + 2 + 3 + \
        4 + 5 + 6 + 7

print "[a={a} / b={b} / c={c} / d={d}]".format(a=a, b=b, c=c, d=d)


########################################################################################################################
# PRINCIPE 3 = on peut séparer une instruction en ne fermant pas un couple de parenthèses/crochets/accolades :
#              (dans ce cas, peu importe l'indentation de la ligne suivante tant qu'elle est légale)
########################################################################################################################
a = (1 + 2 + 3 + 4 + 5 + 6 + 7)

b = (1 + 2 + 3 +
4 + 5 + 6 + 7)

c = (1 + 2 + 3 +
    4 + 5 + 6 + 7)

d = (1 + 2 + 3 +
        4 + 5 + 6 + 7)

print "[a={a} / b={b} / c={c} / d={d}]".format(a=a, b=b, c=c, d=d)


########################################################################################################################
# PRINCIPE 4 = en fait, tant qu'un couple de parenthèses/crochets/accolades, n'est pas fermé, on peut faire plusieurs lignes :
#              (et peu importe l'indentation pour toutes ces lignes)
########################################################################################################################
a = (1 + 2 + 3 + 4 + 5 + 6 + 7)

b = (1 + 2 + 3 +
4 + 5 +
6 +
7)

c = (1 + 2 + 3 +
    4 +
    5 +
    6 + 7)

d = (1 + 2 + 3 +
        4 + 5 +
    6 +
7 +
    8)

print "[a={a} / b={b} / c={c} / d={d}]".format(a=a, b=b, c=c, d=d)

########################################################################################################################
# PRINCIPE 5 = comme tout appel de fonction est entre parenthèse, on peut faire un peu ce qu'on veut avec :-)
#              (again, peu importe l'indentation des morceaux de lignes tant qu'elle est légale)
########################################################################################################################

a_lecole_des_sorciers = "tome 1"
et_la_chambre_des_secrets = "tome 2"
et_le_prisonnier_d_Azkaban = "tome 3"
et_la_coupe_de_feu = "tome 4"
et_l_ordre_du_phoenix = "tome 5"
et_le_prince_de_sang_mele = "tome 6"
et_les_reliques_de_la_mort = "tome 7"

def harry_potter_books(a,b,c,d,e,f,g):
    u""" Fonction prenant beaucoup (7) d'arguments..."""
    print "[" + " / ".join((a,b,c,d,e,f,g)) + "]"

if True:
    # Sans séparation de ligne :
    harry_potter_books(a_lecole_des_sorciers, et_la_chambre_des_secrets, et_le_prisonnier_d_Azkaban, et_la_coupe_de_feu, et_l_ordre_du_phoenix, et_le_prince_de_sang_mele, et_les_reliques_de_la_mort)

    # Séparation, et lignes suivantes au même niveau que l'appel :
    harry_potter_books(a_lecole_des_sorciers, et_la_chambre_des_secrets, et_le_prisonnier_d_Azkaban, et_la_coupe_de_feu,
    et_l_ordre_du_phoenix, et_le_prince_de_sang_mele,
    et_les_reliques_de_la_mort)

    # Séparation, et lignes suivantes au même niveau que la parenthèse :
    harry_potter_books(a_lecole_des_sorciers, et_la_chambre_des_secrets, et_le_prisonnier_d_Azkaban, et_la_coupe_de_feu,
                       et_l_ordre_du_phoenix,
                       et_le_prince_de_sang_mele, et_les_reliques_de_la_mort)

    # Séparation, et lignes suivantes un peu partout :
    harry_potter_books(a_lecole_des_sorciers,
                            et_la_chambre_des_secrets,
                                et_le_prisonnier_d_Azkaban,
                et_la_coupe_de_feu,
    et_l_ordre_du_phoenix,
et_le_prince_de_sang_mele,
    et_les_reliques_de_la_mort)

########################################################################################################################
# Fin du fichier.
########################################################################################################################


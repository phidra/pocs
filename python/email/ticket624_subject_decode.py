#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC de décodage des titres encodés (cf. ticket:624).
#
# À tester avec les DEUX appels suivants (et constater la différence de traitement des espaces des titres problématiques)
#
#     python2.7 ticket624_subject_decode.py
#     python3   ticket624_subject_decode.py
#
#
# Ce que montre la POC :
#     - il y a bien un problème de décodage des headers de mails encodés
#     - c'est surtout un problème python (pas django ou django_mailbox), cf. http://bugs.python.org/issue1079
#     - lorsque tout le mail est encodé, pas de problème
#     - lorsqu'il y a plusieurs parties au mail (certaines encodées, d'autres non, par exemple), problème !
#     - dans ce cas, le nombre d'espaces séparant les différents champs encodés est IGNORÉ par python
#     - (conséquemment, la fonction convert_header_to_unicode de django_mailbox renvoie une valeur erronée)
#     - le problème a été corrigé en python3, mais pas rétroporté en python2.
#
########################################################################################################################

import email.header
import six
DEFAULT_CHARSET = "iso8859-1"

########################################################################################################################
# Fonction utilisée par django_mailbox pour les titres :
# (copiée-collée ici pour la rendre indépendante de django, et pour remplacer le logger par un print)
########################################################################################################################

def convert_header_to_unicode(header):
    def _decode(value, encoding):
        if isinstance(value, six.text_type):
            return value
        if not encoding or encoding == 'unknown-8bit':
            encoding = DEFAULT_CHARSET
        return value.decode(encoding, 'REPLACE')

    try:
        return ''.join(
            [
                (
                    _decode(bytestr, encoding)
                ) for bytestr, encoding in email.header.decode_header(header)
            ]
        )
    except UnicodeDecodeError:
        print(u"Errors encountered decoding header {} into encoding {}.".format(header, DEFAULT_CHARSET))
        return unicode(header, DEFAULT_CHARSET, 'replace')

########################################################################################################################
# Essais sur des titres encodés :
########################################################################################################################

titres = {}
titres[u"titre_normal         "] = "Je suis un titre normal                                                                      "
titres[u"titre_problematique_1"] = "Avant encodage=?UTF-8?B?cGnDqGNlLWpvaW50ZQ==?=                                               "
titres[u"titre_problematique_2"] = "Avant encodage =?UTF-8?B?cGnDqGNlLWpvaW50ZQ==?=                                              "
titres[u"titre_problematique_3"] = "Avant encodage     =?UTF-8?B?cGnDqGNlLWpvaW50ZQ==?=                                          "
titres[u"titre_ok             "] = "=?ISO-8859-1?Q?Gmail_est_diff=E9rent_des_autres_syst=E8mes_de_?= =?ISO-8859-1?Q?messagerie.?="

for key, value in six.iteritems(titres):
    print(key)
    print(u"\t[INITIAL={}]".format(value))
    print(u"\t[DECODED={}]".format(convert_header_to_unicode(value)))


########################################################################################################################
# Fin du fichier.
########################################################################################################################

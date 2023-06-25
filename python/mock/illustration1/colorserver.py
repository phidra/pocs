#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
# Ce serveur web fournit une couleur en réponse à chaque requête.
# La couleur varie parmi black / white / red / green (selon une mécanique interne au serveur).
#
# Ce serveur est interrogé par un client, qui constitue la POC.
# C'est pour implémenter les tests unitaires du client qu'on va utiliser unittest.mock.
# En effet, on va mocker le présent serveur.
#
########################################################################################################################
import datetime

def response_builder():
    """
    Construit la réponse à renvoyer.
    La réponse renvoyée varie en fonction du dernier chiffres des secondes actuelles (>5 ou non).
    La couleur est choisie par une stratégie qui change avec la parité des secondes actuelles.
    """
    seconds = datetime.datetime.now().second
    lastdigit = int(str(seconds)[-1])
    switch_condition = lastdigit >=5

    color_strategy = lambda switch_condition: "green" if switch_condition else "red"
    gray_strategy = lambda switch_condition: "white" if switch_condition else "black"

    if seconds % 2 == 0:
        return color_strategy(switch_condition)
    else:
        return gray_strategy(switch_condition)

########################################################################################################################

import socket

HOST, PORT = '', 8888

listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
listen_socket.bind((HOST, PORT))
listen_socket.listen(1)
print 'Serving HTTP on port %s ...' % PORT
while True:
    client_connection, client_address = listen_socket.accept()
    request = client_connection.recv(1024)
    print request

    http_response = """\
HTTP/1.1 200 OK

{response}
""".format(response=response_builder())
    client_connection.sendall(http_response)
    client_connection.close()

########################################################################################################################
# Fin du fichier.
########################################################################################################################

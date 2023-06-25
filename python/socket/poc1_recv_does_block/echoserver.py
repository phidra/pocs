#!/usr/bin/env python3
import socket
import logging
import sys
from datetime import datetime

"""
Implémentation (à l'aide de la lib standard python pour travailler avec des sockets) d'un echoserver simple.
Le serveur accepte un client à la fois, et logge un message avant et après chaque recv.

HOWTO :
    dans un premier terminal - lancer le serveur :
        python3.6 echoserver.py 50007  # ou n'importe quel autre port libre
    dans un second  terminal - lancer un client :
        nc localhost 50007             # connecter le client. À partir de maintenant, on est dans une session netcat
    puis, envoyer à loisir des données sur la socket :
        pouet coucou                   # envoi de données 1 -> ça débloque recv
        youpi tralala                  # envoi de données 2 -> ça débloque recv de nouveau
        CTRL+D                         # fermer la connexion TCP

Cette POC montre que l'appel de recv est BLOQUANT, et se débloque dès que le client envoie des données.
"""

logging.basicConfig(format='[%(asctime)s]   %(message)s', level=logging.INFO)

HOST = ''
try:
    PORT = int(sys.argv[1])
except IndexError:
    PORT = 50007  # Arbitrary non-privileged port

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    while True:
        logging.info(f"waiting for a client to connect on '{HOST}:{PORT}'")
        conn, addr = s.accept()
        with conn:
            logging.info(f"connected by client {addr[0]}:{addr[1]}")
            while True:
                logging.info(f"about to recv, which will BLOCK, waiting for data...")
                before = datetime.now()
                data = conn.recv(1024)
                after = datetime.now()
                seconds_blocked = round((after-before).total_seconds(), 1)
                if not data:
                    logging.info(f"!!!UNBLOCKING AFTER {seconds_blocked} seconds!!! : disconnecting client")
                    break
                msg_length = len(data)
                logging.info(f"!!!UNBLOCKING AFTER {seconds_blocked} seconds!!! : received {msg_length} bytes")
                conn.sendall(b"PREFIX : " + data)
        logging.info(f"client disconnected...\n")

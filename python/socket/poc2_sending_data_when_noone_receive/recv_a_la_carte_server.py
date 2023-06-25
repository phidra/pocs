#!/usr/bin/env python3
import socket
import logging
import sys
from datetime import datetime

"""
HOWTO :
    dans un terminal 1 = lancer le serveur : il n'appelle recv QUE lorsque l'utilisateur le lui demande
    dans un terminal 2 = lancer le client  : il envoie INDÉFINIMENT des données (1kio par 1kio)

Même sans AUCUN revc, le client envoie la bouche en coeur ses données (ils vont dans le buffer de l'OS)
Toujours sans aucun recv, au bout d'un moment, le client finit par bloquer et patienter (après avoir envoyé 2625 fois 1kio, semble répétable)
Si on trigge revc sur le serveur (il faut le trigger plusieurs fois, apparemment), ça débloque le client...
... jusqu'à ce qu'on remplisse de nouveau le buffer de l'OS : le client bloque alors de nouveau en patientant.

APPRIS :
    1. il est tout à fait possible pour la pile TCP/IP de recevoir des données même si la connected-socket du serveur n'appelle pas recv
    2. du coup, on peut très bien faire uniquement "un recv de temps en temps" pour vider le buffer de l'OS
    3. attention : si on fait trop peu de recv, on sature le receive buffer
    4. même si le moment où on sature est répétable, je n'arrive pas trop à le corréler aux valeurs de /proc/sys/net/core/rmem_max ?!
    5. lorsqu'on débloque le client, il se remet à envoyer des données, et bloque beaucoup plus tard (compteur ~ 10000) : peut-être que la sliding-window TCP a agumenté de taille entretemps ?
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
                logging.info(f"I will now sleep : when you want to wake me up so that I recv, hit ENTER...")
                input("Please hit ENTER when you are ready...")
                logging.info(f"I will now recv (and maybe block, if no data is available)")
                before = datetime.now()
                data = conn.recv(10485760)
                after = datetime.now()
                seconds_blocked = round((after-before).total_seconds(), 1)
                if not data:
                    logging.info(f"!!!UNBLOCKING AFTER {seconds_blocked} seconds!!! : disconnecting client")
                    break
                msg_length = len(data)
                logging.info(f"!!!UNBLOCKING AFTER {seconds_blocked} seconds!!! : received {msg_length} bytes")
        logging.info(f"client disconnected...\n")

Cette POC montre que le syscall read utilise le page-cache.

Ce que fait la POC :
    - elle lit en boucle (syscall read) les 1024 premiers octets d'un fichier d'environ 1Mio
    - elle affiche le temps nécessaire à la lecture
    - en cours de boucle, dans un autre terminal, je vide le page-cache

Ce que je constate :
    - le temps moyen pour lire le fichier est de l'ordre de la microseconde ~15 µs
    - exception : pour l'itération qui suit le fait d'avoir vidé le page-cache, le temps nécessaire est multiplié par environ 1000 (de l'ordre de la ms)

Ce que j'en déduis :
    - ça confirme que le syscall read ne lit pas nécessairement sur le disque, mais passe par le page-cache (ce qui est logique)

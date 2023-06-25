# cas 1 = stopped container :

# que fait le Dockerfile :
bat Dockerfile

# repartir d'un état propre :
docker system prune -a

# pas de containers, pas d'images :
docker image list -a
docker container list -a

# builder l'image :
docker build -t mycontainer .


# toujours pas de containers, mais des images :
docker image list -a
docker container list -a
# (on reviendra plus tard sur les images)

# exécution du container :
docker run mycontainer

# ce coup-ci, on a un container arrêté :
docker container list
docker container list -a


# et même plusieurs, possiblement :
docker run mycontainer
docker run mycontainer
docker run mycontainer
docker container list -a

# pruning = supprimer les containers arrêtés :
docker container prune
docker container list -a

# option --rm
docker run --rm mycontainer
docker run --rm mycontainer
docker run --rm mycontainer
docker container list -a

# attention : workflow arrêt/redémarrage !

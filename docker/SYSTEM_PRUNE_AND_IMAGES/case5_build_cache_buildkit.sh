# cas 5 = c'est quoi le "all build cache" du docker system prune ?

# que fait le Dockerfile :
bat Dockerfile

# créer des images dangling, elles ne sont PAS supprimées par "docker builder prune" :
docker system prune -a
docker image list -a
docker build -t mycontainer .
vim Dockerfile  # modifier les layers
docker build -t mycontainer .  # on déplace le tag !
docker image list -a

docker system df
# TYPE            TOTAL     ACTIVE    SIZE      RECLAIMABLE
# Images          2         0         4.863MB   4.863MB (100%)
# Containers      0         0         0B        0B
# Local Volumes   2         0         0B        0B
# Build Cache     0         0         0B        0B

# les nouvelles images intermédiaires sont utiles à l'image taggée... mais les anciennes images intermédiaires ne servent plus !
docker builder prune  # aucune suppression...
docker image prune  # suppression des images !

# CONCLUSION TEMPORAIRE : "all build cache" ne concerne pas les images intermédiaires (aka le "cache") ?!

# idem, avec DOCKER_BUILDKIT=1 :
docker system prune -a
docker image list -a
docker system df
# TYPE            TOTAL     ACTIVE    SIZE      RECLAIMABLE
# Images          0         0         0B        0B
# Containers      0         0         0B        0B
# Local Volumes   2         0         0B        0B
# Build Cache     0         0         0B        0B


DOCKER_BUILDKIT=1 docker build -t mycontainer .
docker system df
# TYPE            TOTAL     ACTIVE    SIZE      RECLAIMABLE
# Images          1         0         4.863MB   4.863MB (100%)
# Containers      0         0         0B        0B
# Local Volumes   2         0         0B        0B
# Build Cache     8         0         269B      269B

docker image list -a
# REPOSITORY    TAG       IMAGE ID       CREATED          SIZE
# mycontainer   latest    581fb0061bcd   19 seconds ago   4.86MB

# CONCLUSION TEMPORAIRE = le cache est lié à DOCKER_BUILDKIT, qui n'utilise plus d'images intermédiaires comme cache.

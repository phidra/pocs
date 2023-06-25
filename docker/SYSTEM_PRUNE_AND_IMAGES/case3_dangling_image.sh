# cas 3 = dans quel cas a-t-on une image dangling ?

# que fait le Dockerfile :
bat Dockerfile

# repartir d'un état vierge :
docker system prune -a
docker image list -a

# builder l'image → pruner ne supprime pas notre image (ni les images intermédiaires) :
docker build -t mycontainer .
docker system prune
docker image list
docker image list -a

# repartir d'un état vierge :
docker system prune -a
docker image list -a

# builder l'image SANS TAG → pruner supprime notre image :
docker build .
docker image list  # image SANS TAG
docker run 04f977368bf9
docker system prune
docker image list

# CONCLUSION TEMPORAIRE : une image dangling, c'est une image sans tag !
# QUESTION : sachant qu'on builde quasi-toujours avec des tags, comment apparaissent-elles ?
# QUESTION : pourquoi nos images intermédiaires n'étaient elles pas dangling :

# les images intermédiaires ne sont pas dangling :
docker system prune -a
docker image list -a
docker build -t mycontainer .
docker image list -a
docker system prune
docker image list -a  # images intermédiaires non-supprimées, malgré l'absence de tag

# explication = une image non-dangling a une image intermédiaire comme parente :
docker inspect mycontainer | less

# CONCLUSION : une image dangling, c'est une image sans tag et qui n'est pas parente d'une image taggée !
# (docker system prune ne cleanera donc pas le "cache" qui sert à rebuilder une image actuellement taggée)

# comment une image dangling apparaît-elle ?
docker system prune -a
docker image list -a
docker build -t mycontainer .
docker system prune  # rien n'est supprimé : les images intermédiaires sont utiles à l'image taggée
vim Dockerfile  # modifier les layers
docker build -t mycontainer .  # on déplace le tag !
# les nouvelles images intermédiaires sont utiles à l'image taggée... mais les anciennes images intermédiaires ne servent plus !
docker system prune  # les images dangling sont supprimées

# CONCLUSION : les images dangling apparaissent quand on rebuilde des images / déplace des tags.

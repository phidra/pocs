# cas 4 = c'est quoi une "unreferenced image"

# que fait le Dockerfile :
bat Dockerfile

# repartir d'un état vierge :
docker system prune -a
docker image list -a

# builder l'image, et l'utiliser (en la gardant vivante) :
docker build -t mycontainer .
docker run -it mycontainer /bin/ash


# dans un autre terminal :
docker system prune -a
docker image list
# l'image étant utilisée, elle n'est pas supprimée

# killer le container, et réessayer :
docker system prune -a
docker image list
# l'image n'étant plus utilisée, elle a été supprimée

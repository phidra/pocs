# cas 2 = chaque ligne d'un dockerfile est une image :

# que fait le Dockerfile :
bat Dockerfile

# repartir d'un état propre :
docker system prune -a

# pas de containers, pas d'images :
docker image list -a

# builder l'image :
docker build -t mycontainer .

# deux images :
docker image list

# mais en fait plus que ça :
docker image list -a

# les images "anonymes" sont des images intermédiaires :
docker inspect mycontainer | less
docker inspect d90f0cc26e87 | less

# ces images intermédiaires ne sont pas supprimées par un prune :
docker system prune
docker image list -a

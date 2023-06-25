# cas 6 = comment fonctionne le cache de buildkit

# que fait le Dockerfile :
bat Dockerfile

# créer une image avec BuildKit :
docker system prune -a
docker image list -a
DOCKER_BUILDKIT=1 docker build -t mycontainer .

docker buildx du
# ID                                              RECLAIMABLE     SIZE            LAST ACCESSED
# jdohv39po77mqjejx8l12ec5u*                      true            264B            2 seconds ago
# svsqeyqouwxr1x10s75ywjc6p                       true            6B*             2 seconds ago
# sdezlemwzi6w81o609oogmtg9                       true            6B*             2 seconds ago
# mdcs2oo9nyyh52zbe7rhyphvl                       true            6B*             2 seconds ago
# adoru2t4s63hh45lvs2i8x3kd                       true            6B*             2 seconds ago
# 6cc7ue8zl4ysnk87atix429gq                       true            6B*             2 seconds ago
# xsdgjfijszp2tjv9pgmjiaeww*                      true            0B              2 seconds ago
# uiugud8clq10ca3t1gf3topwb                       true            0B*             2 seconds ago
# Shared:         30B
# Private:        264B
# Reclaimable:    294B
# Total:          294B

docker buildx du --verbose

docker buildx du --verbose|grep Description
# Description:    local source for dockerfile
# Description:    mount / from exec /bin/sh -c echo "hoho5" > /tmp/pouet5
# Description:    mount / from exec /bin/sh -c echo "hoho4" > /tmp/pouet4
# Description:    mount / from exec /bin/sh -c echo "hoho1" > /tmp/pouet1
# Description:    mount / from exec /bin/sh -c echo "hoho2" > /tmp/pouet2
# Description:    mount / from exec /bin/sh -c echo "hoho3" > /tmp/pouet3
# Description:    pulled from docker.io/library/busybox:latest@sha256:c118f538365369207c12e5794c3cbfb7b042d950af590ae6c287ede74f29b7d4
# Description:    local source for context

docker builder prune
# Les deux objets supprimés même immédiatement après un build sont le context et le dockerfile :
# Description:    local source for context
# Description:    local source for dockerfile

docker builder prune

# CONCLUSION TEMPORAIRE = le context et le dockerfile (pas réutilisables) sont prunés. Les autres objets sont conservés.



# Les autres entrées sont les layers intermédiaires, elles sont supprimées lorsque l'image taggée correspondante est supprimée :
docker image rm mycontainer
docker buildx du  # pas encore supprimé
docker builder prune
docker buildx du  # supprimé


# CONCLUSION :
# En résumé, le cache est associé à une image
# Le cache devient dangling quand l'image est supprimée
# Tant que l'image existe, le cache est conservé

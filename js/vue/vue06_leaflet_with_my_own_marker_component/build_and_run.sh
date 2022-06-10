#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

ATTENTION : cette POC est ABANDONNÉE et n'est pas utilisable !
ATTENTION : cette POC est ABANDONNÉE et n'est pas utilisable !
ATTENTION : cette POC est ABANDONNÉE et n'est pas utilisable !

Cette POC était un essai de construire une app leaflet avec des composants vue réutilisables.

Ce que je voulais faire du point de vue fonctionnel :
    - avoir une carte et deux marqueurs sur la carte (rouge et vert ?)
    - quand j'en déplace un, ça fait un console.log avec les positions des deux marqueurs
    - l'URL encode dynamiquement la position de la carte (bonus : l'URL comporte la position des marqueurs)

Ce que je voulais faire du point de vue technique :
    - faire mes propres composants vue (plutôt que p.ex. utiliser vue-leaflet)
    - un composant App.vue qui contient l'essentiel de l'app
    - un composant Marker.vue qui contient un marker et sa position
    - (un éventuel composant Map.vue pour la carte, mais dans un premier temps, la map peut être dans l'app)
    - le déplacement du marqueur doit être loggé par le composant parent

Status en résumé = il faudrait que je maintienne un mapping entre les structures leaflet et les composants vue.

C'est pas impossible techniquement (c'est ce que fait vue-leaflet = https://github.com/vue-leaflet/vue-leaflet)

Mais comme c'est trop compliqué au vu de mon besoin, j'abandonne la POC...

Une meilleure POC pour faire une app avec vue et leaflet consisterait à :
    - avoir un seul composant vue qui utilise leaflet (et on laisse leaflet gérer les objets liés à la map comme les marqueurs)
    - avoir d'autres composants pour le reste de l'app (e.g. si je veux avoir un panel sur la map)

(en commentaire du présent script, j'ai laissé mes notes brutes)

ATTENTION : cette POC est ABANDONNÉE et n'est pas utilisable !
ATTENTION : cette POC est ABANDONNÉE et n'est pas utilisable !
ATTENTION : cette POC est ABANDONNÉE et n'est pas utilisable !

EOF


# QUESTION : mon composant Marker va avoir besoin de l'objet map leaflet (pour faire addTo), comment faire ça ?
# 	solution 1 = je passe la map leaflet en paramètre de mon composant (c'est pas beau)
# 	solution 2 = le composant se contente de gérer son état interne, et le parent est en charge de faire le addTo
# 	question : comment fait vue-leaflet ?
# 		il initialise un objet vide leafLetRef dont il prend une ref
# 			https://vuejs.org/api/reactivity-core.html#ref
# 		il appelle markerSetup (qui est juste la fonction setup de functions/marker.js) en lui passant cette ref
# 		on dirait que tout ajout passe par layerSetup fonction setup du fichier ./functions/layer.js)
# 		utile :
# 			https://vuejs.org/guide/components/provide-inject.html
# 			https://vuejs.org/guide/essentials/lifecycle.html#lifecycle-diagram
# 		a priori, ça répond à ma question :
# 			layer.addTo(map), qui est ce que j'utilise, ajoute un layer à une map = https://leafletjs.com/reference.html#layer-addto
# 			map.addLayer(layer) est ce qu'utilise vue.leaflet = https://leafletjs.com/reference.html#map-addlayer
# 			le component LMap "provide" la fonction addLayer = https://github.com/vue-leaflet/vue-leaflet/blob/852a180777870bd8982c78d2d74d5282f187d9a9/src/components/LMap.vue#L185 (via un leaflet wrapper que je maîtrise mal)
# 			le component LMarker "inject" addLayer = https://github.com/vue-leaflet/vue-leaflet/blob/852a180777870bd8982c78d2d74d5282f187d9a9/src/components/LMarker.vue#L24
# 			ainsi, le composant fils peut s'ajouter tout seul à la map (qui est un composant très haut dans la chaîne)
#
# REPRISE DEPRECATED = utilise le mécanisme de inject/provide pour qu'à sa création, mon composant MyOwnMarker créée un marker leaflet + l'ajoute sur un layer de la map
# 	(c'est le mécanisme utilisé par vue-leaflet pour que le LMarker puisse s'ajouter sur la LMap, cf. notes du 10 juin)
# 	https://vuejs.org/guide/components/provide-inject.html#prop-drilling
#
# REPRISE (la vraie) = il me faudra impérativement utilise vue-leaflet, donc ma tâche de reprise = revenir à mon objectif d'utiliser vue-leaflet pour faire ce que je veux :
# 	une map + un marker + un contrôle
# 	quand je bouge le marker, j'update le contrôle
# 	(la partie difficile sera sans doute de définir un contrôle custom avec vue-leaflet... peut-être utiliser des sous-composants ?)
# 		https://vue2-leaflet.netlify.app/examples/custom-control.html
#
# J'ai commencé à reprendre :
# 	j'arrive à faire passer un message via inject/provide depuis le composant parent vers le composant enfant
# 	problème : pour créer la map, j'ai besoin que le HTML soit déjà rendu (car la map leaflet est créée sur un div avec un id), mais le HTML du parent ne sera créé que lorsque l'enfant sera créé... or l'enfant a besoin de l'objet map du parent !
# 	solution temporaire = créer le marker dynamiquement
# 	question : comment fait vue-leaflet ?
# 		https://github.com/vue-leaflet/vue-leaflet/blob/852a180777870bd8982c78d2d74d5282f187d9a9/src/components/LMarker.vue#L47
# 			déjà, le marker est créé sans l'ajouter à quoi que ce soit pour le moment
# 		https://github.com/vue-leaflet/vue-leaflet/blob/852a180777870bd8982c78d2d74d5282f187d9a9/src/components/LMarker.vue#L54
# 			puis, on l'ajoute en appelant addLayer
# 		https://github.com/vue-leaflet/vue-leaflet/blob/852a180777870bd8982c78d2d74d5282f187d9a9/src/components/LMap.vue#L185
# 			la fonction addLayer a été redéfinie par Map (puis provided à ses fils), ce n'est donc pas directement addLayer qui est appelé
# 		https://github.com/vue-leaflet/vue-leaflet/blob/852a180777870bd8982c78d2d74d5282f187d9a9/src/components/LMap.vue#L254
# 			et ce que fait le addLayer remplaçant, c'est de gérer des "blueprints"
# 		tout ça me conduit à penser sans en être sûr que vue-leaflet maintient un mécanisme complexe permettant de gérer la dualité entre leaflet d'un côté et les composants vue de l'autre...
# 		du coup, refaire ça à la main (i.e. avoir une hiérarchie de composants vue qui mimiquent la hiérarchie d'objets leaflet) sera sans doute trop compliqué...
# 	conclusion : soit j'utilise vue-leaflet, soit le jeu n'en vaut pas la chandelle...

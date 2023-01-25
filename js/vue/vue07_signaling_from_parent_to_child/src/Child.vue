<template>
  <!-- ici, on utilise directement la props pour que le parent pilote l'affichge du composant enfant -->
  <!-- (ça n'est pas tout à fait ce que je veux, car je veux plutôt pouvoir appeler une fonction) -->
  <h2 v-show="personToGreet != 'UNKNOWN'"> Child using v-show : coucou... {{ personToGreet }} </h2>
</template>

<script>
export default {
  props: {
    personToGreet: String,
  },
}
</script>

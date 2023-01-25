<script setup>
import Parent from './Parent.vue'
</script>

<template>
  <main>
    <p>
      L'objectif de cette POC est de montrer comment un composant parent peut trigger un event chez un composant enfant.
    </p>
    <p>
      Le contexte est que je veux utiliser leaflet dans une application vue, que j'ai donc un composant Map (parent) qui va contenir des Markers (enfants), mais que les enfants ont besoin d'être construits tardivement : juste après qu'on ait fini de construire la map leaflet qui les accueillera.
    </p>
    <p>
      On ne peut pas utiliser le cycle de vie classique des composants pour ça, car un composant parent termine son initialisation après ses enfants.
    </p>
    <br />
    <p>
      TL;DR = il ne faut pas  utiliser <code>emits</code> (qui est réservé à le commnication Child → Parent), mais plutôt utiliser les <code>props</code> (qui sont la bonne façon de communiquer dans le sens Parent → Child) couplés avec un <b>watcher</b> pour trigger un évènement lorsque la prop change.
    </p>
    <hr />
      <Parent />
  </main>
</template>

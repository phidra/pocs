<template>
  <!-- ici, on a ce que je veux = le parent est capable de trigger une fonction chez le child -->
  <!-- (et j'utilise cette fonction pour démasquer le h2) -->
  <h2 id="child2" style="display: none;"> Child using watcher : coucou... {{ personToGreet }} </h2>
</template>

<script>
export default {
  props: {
    personToGreet: String,
  },
  watch: {
    personToGreet() {
      this.on_person_to_greet_set();
    }
  },
  methods: {
    on_person_to_greet_set() {
      if (this.personToGreet !== "UNKNOWN") {
        document.getElementById("child2").style.display = "block";
      }
    }
  },
}
</script>

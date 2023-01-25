<template>
  <h1> Parent : my name is {{ name }}. </h1>
  <h2 v-show="name == 'UNKNOWN'"> Setting name in... {{ status }} </h2>
  <Child :person-to-greet="name" />
  <Child2 :person-to-greet="name" />
</template>

<script>
import Child from './Child.vue';
import Child2 from './Child2.vue';
export default {
  data() {
    return {
      name: "UNKNOWN",
      status: "?",
    }
  },
  mounted() {
    setTimeout(() => { this.status = "9" }, 1000);
    setTimeout(() => { this.status = "8" }, 2000);
    setTimeout(() => { this.status = "7" }, 3000);
    setTimeout(() => { this.status = "6" }, 4000);
    setTimeout(() => { this.status = "5" }, 5000);
    setTimeout(() => { this.status = "4" }, 6000);
    setTimeout(() => { this.status = "3" }, 7000);
    setTimeout(() => { this.status = "2" }, 8000);
    setTimeout(() => { this.status = "1" }, 9000);
    // l'objectif est de trigger un évènement chez les enfants
    // (le point important = on veut trigger l'évènement à un moment quelconque APRÈS la création)
    setTimeout(() => { this.name = "Luke SKYWALKER" ; this.status = "DONE" }, 10000);
  },
  components: {
    Child,
    Child2,
  },
}
</script>

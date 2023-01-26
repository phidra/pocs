<script setup>
import MyMap from './components/MyMap.vue'
import MyResults from './components/MyResults.vue'
</script>

<template>
  <main>
    <h1>Ma super map</h1>
      <MyResults />
      <MyMap />
  </main>
</template>

<style>
@import './assets/base.css';

#app {
  margin: 0 auto;
  font-weight: normal;
}

header {
  line-height: 1.5;
}

</style>

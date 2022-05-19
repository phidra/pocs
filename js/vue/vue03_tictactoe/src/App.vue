<script setup>
import Tictactoe from './components/Tictactoe.vue'
</script>

<template>
  <header>
    <img alt="Vue logo" class="logo" src="./assets/logo.svg" width="125" height="125" />
  </header>

  <main>
      <p class="status"> Voici le super tictactoe :</p>
      <Tictactoe />
  </main>
</template>

<style>
@import './assets/base.css';

#app {
  margin: 0 auto;
  padding: 2rem;

  font-weight: normal;
}

header {
  line-height: 1.5;
}

.logo {
  display: block;
  margin: 0 auto 2rem;
}

</style>

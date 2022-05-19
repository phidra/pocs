<script>
export default {
  name: "Cell",
  data() {
    return {
    }
  },
  props: {
    id: Number,
    value: String,
  },
  methods: {
    notify() {
      // ignore clicks on already filled cells :
      if (this.value !== "") {
        return;
      }
      this.$emit("filled", this.id);
    },
  },
}
</script>

<template>
  <button class="square" @click="notify"> {{ value }} </button>
</template>

<style scoped>
</style>


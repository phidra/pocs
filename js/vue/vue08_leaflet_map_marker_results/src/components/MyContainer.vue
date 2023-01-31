<template>
    <MyResults :marker_lat="marker_lat" :marker_lng="marker_lng" />
    <MyMap @marker-moved="on_marker_moved" />
</template>

<script>
import MyMap from './MyMap.vue'
import MyResults from './MyResults.vue'
export default {
  name: "Container",
  components: {
    MyMap,
    MyResults,
  },
  data() {
    return {
      marker_lat: 0,
      marker_lng: 0,
    };
  },
  methods: {
    on_marker_moved(latlng) {
      console.log("MyContainer : marker is moved to : ", latlng);
      this.marker_lat = latlng.lat;
      this.marker_lng = latlng.lng;
    },
  },
};
</script>

<style scoped>
</style>

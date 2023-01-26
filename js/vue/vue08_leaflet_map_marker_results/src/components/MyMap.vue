<template>
  <div id="mapContainer"></div>
  <MyMarker :is-map-created="is_map_created" :leaflet-map="leaflet_map"/>
</template>

<script>
import MyMarker from './MyMarker.vue';
import "leaflet/dist/leaflet.css";
import L from "leaflet";


export default {
  name: "LeafletMap",
  data() {
    return {
      is_map_created: false,
      leaflet_map: null,
    };
  },
  mounted() {
    const DEFAULT_CENTER = [48.8495, 2.3568];  // PARIS
    const DEFAULT_ZOOM = 12;
    let [center, zoom, markerpos] = [DEFAULT_CENTER, DEFAULT_ZOOM, DEFAULT_CENTER];
    console.log(`using map zoom ${zoom}`);
    console.log(`using map center ${center}`);
    console.log(`using makerpos ${markerpos}`);

    this.leaflet_map = L.map("mapContainer");
    this.leaflet_map.setView(center, zoom);
    this.is_map_created = true;
    console.log("map created");


    L.tileLayer("http://{s}.tile.osm.org/{z}/{x}/{y}.png", {
      attribution:
        '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors',
    }).addTo(this.leaflet_map);
  },
  components: {
    MyMarker,
  }
};
</script>

<style scoped>
#mapContainer {
  width: 100vw;
  height: 100vh;
}
</style>


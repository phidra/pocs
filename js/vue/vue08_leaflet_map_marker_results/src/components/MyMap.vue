<template>
  <div id="mapContainer"></div>
  <MyMarker :is-map-created="is_map_created" :leaflet-map="leaflet_map" @marker-moved="on_marker_moved"/>
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
  emits: ["marker-moved"],
  mounted() {
    const center = [48.8495, 2.3568];  // PARIS
    const zoom = 12;
    this.leaflet_map = L.map("mapContainer");
    this.leaflet_map.setView(center, zoom);
    this.is_map_created = true;


    L.tileLayer("http://{s}.tile.osm.org/{z}/{x}/{y}.png", {
      attribution:
        '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors',
    }).addTo(this.leaflet_map);
  },
  components: {
    MyMarker,
  },
  methods: {
    on_marker_moved(latlng) {
      console.log("MyMap : marker is moved to : ", latlng);
      this.$emit("marker-moved", latlng);
    },
  },
};
</script>

<style scoped>
#mapContainer {
  width: 100vw;
  height: 100vh;
}
</style>


<template>
  <div id="mapContainer"></div>
</template>

<script>
import "leaflet/dist/leaflet.css";
import L from "leaflet";
import {parse_current_url_params} from "./url.js";

export default {
  name: "LeafletMap",
  data() {
    return {
      map: null,
    };
  },
  mounted() {


    const DEFAULT_CENTER = [48.8495, 2.3568];  // PARIS
    const DEFAULT_ZOOM = 12;
    let [center, zoom, markerpos] = parse_current_url_params(DEFAULT_CENTER, DEFAULT_ZOOM);
    if (markerpos === null) {
      console.log("setting default for marker position");
      markerpos = DEFAULT_CENTER;
    }

    console.log(`using map zoom ${zoom}`);
    console.log(`using map center ${center}`);
    console.log(`using makerpos ${markerpos}`);

    this.map = L.map("mapContainer").setView(center, zoom);


    L.tileLayer("http://{s}.tile.osm.org/{z}/{x}/{y}.png", {
      attribution:
        '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors',
    }).addTo(this.map);
    L.marker(markerpos, {draggable: true}).addTo(this.map);
  },
  onBeforeUnmount() {
    if (this.map) {
      this.map.remove();
    }
  },
};
</script>

<style scoped>
#mapContainer {
  width: 100vw;
  height: 100vh;
}
</style>

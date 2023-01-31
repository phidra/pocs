<script>
import L from "leaflet";
export default {
  name: "MyMarker",
  props: {
    isMapCreated: Boolean,
    leafletMap: Object,
  },
  data() {
    return {
      leaflet_marker: null,
    }
  },
  emits: ["marker-moved"],
  methods: {
    create_marker(leafletMap) {
      const DEFAULT_CENTER = [48.8495, 2.3568];  // PARIS
      this.leaflet_marker = new L.Marker(DEFAULT_CENTER, {draggable: true});
      this.leaflet_marker.addTo(leafletMap);
      this.leaflet_marker.on("moveend", () => {
        this.$emit("marker-moved", this.leaflet_marker.getLatLng());
      });
    },
  },
  watch: {
    isMapCreated() {
      if (this.isMapCreated === true) {
        console.log("map is created -> about to create marker");
        this.create_marker(this.leafletMap);
        // manually trigger marker moved at startup, in order to display its position in results :
        this.$emit("marker-moved", this.leaflet_marker.getLatLng());
      }
    }
  },
}
</script>

<template>
</template>

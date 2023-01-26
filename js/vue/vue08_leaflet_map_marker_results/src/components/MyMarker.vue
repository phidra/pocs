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
      lat: 0,
      lng: 0,
      leaflet_marker: null,
    }
  },
  mounted: function() {
    console.log("mounted");
    console.log("isMapCreated = ", this.isMapCreated);
    console.log("map = ", this.map);
  },
  methods: {
    create_marker(leafletMap) {
      console.log("creating marker");
      const DEFAULT_CENTER = [48.8495, 2.3568];  // PARIS
      const marker = new L.Marker(DEFAULT_CENTER, {draggable: true});
      marker.addTo(leafletMap);
    },
  },
  watch: {
    isMapCreated() {
      if (this.isMapCreated === true) {
        console.log("SIGNALED !!!");
        console.log("isMapCreated = ", this.isMapCreated);
        console.log("map = ", this.leafletMap);
        this.create_marker(this.leafletMap);
      }
    }
  },
}
</script>

<template>
</template>

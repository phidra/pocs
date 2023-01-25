<template>
  <div style="height: 75vh; width: 50vw;">
    <l-map
      v-model:zoom="zoom"
      :zoomAnimation="true"
      :center="[47.41322, -1.219482]"
      @move="log('move')"
      >
      <l-tile-layer
        url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        ></l-tile-layer>
      <l-control-layers />
      <l-marker :lat-lng="[47.41322, -1.219482]" draggable @moveend="log('moveend')">
        <l-icon :icon-url="iconUrl" :icon-size="iconSize" />
      </l-marker>
      <l-control position="bottomleft" >
        <button @click="clickHandler">
          Click me to change kitten !
        </button>
      </l-control>
    </l-map>
  </div>
</template>

<script>
import {
  LMap,
  LIcon,
  LControl,
  LTileLayer,
  LMarker,
  LControlLayers,
} from "@vue-leaflet/vue-leaflet";
import "leaflet/dist/leaflet.css";
export default {
  components: {
    LMap,
    LIcon,
    LControl,
    LTileLayer,
    LMarker,
    LControlLayers,
  },
  data() {
    return {
      zoom: 5,
      iconWidth: 25,
      iconHeight: 40,
    };
  },
  computed: {
    iconUrl() {
      return `https://placekitten.com/${this.iconWidth}/${this.iconHeight}`;
    },
    iconSize() {
      return [this.iconWidth, this.iconHeight];
    },
  },
  methods: {
    log(a) {
      console.log(a);
    },
    clickHandler () {
      this.iconWidth += 20;
      if (this.iconWidth > this.iconHeight) {
        this.iconWidth = Math.floor(this.iconHeight / 2);
      }
    },
  },
};
</script>

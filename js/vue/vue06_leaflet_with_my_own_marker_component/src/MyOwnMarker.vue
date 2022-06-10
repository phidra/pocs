<script>
import L from "leaflet";
export default {
  name: "Cell",
  data() {
    return {
      lat: 0,
      lng: 0,
      marker_object: null,
    }
  },
  inject: ['map', 'message'],
  mounted: function() {
    alert(`message is ${this.message}, est-ce que la map est null ? ${this.map === null}`);
    L.marker([42, 1], {draggable: true}).addTo(this.map);
  },
  methods: {
    on_start_new_game() {
      this.lat = 42;
      this.lng = 1;
    },
  },
}
</script>

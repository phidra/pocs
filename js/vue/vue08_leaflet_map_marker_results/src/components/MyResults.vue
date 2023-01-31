<script>
export default {
  name: "MyPosition",
  props: {
    marker_lat: {type: Number, default: -1},
    marker_lng: {type: Number, default: -1},
  },
}
</script>

<template>
  <p> <b>RESULTS = </b> marker is at lat={{ marker_lat }} ; lng={{ marker_lng }} </p>
</template>

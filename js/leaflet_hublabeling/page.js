// POC fortement inspirée de : https://leafletjs.com/examples/choropleth/
import {initMap} from "./map.js";
import {displayGeojson} from "./geojson.js";
import {mocked_data} from "./data.js";

const [the_map, info] = initMap();
displayGeojson(the_map, info, mocked_data);

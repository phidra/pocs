// POC fortement inspirée de : https://leafletjs.com/examples/choropleth/
import {initMap} from "./map.js";
import {initControls} from "./controls.js";
import {loadGeojsonFromFile, loadMockedGeojson, unloadGeojson, clearUploadedFile} from "./geojson.js";

const the_map = initMap();
const [infoControl, loadMockedButton, loadFromFileButton, unloadButton] = initControls(the_map);

// button to load data from uploaded geojson file :
loadFromFileButton.setChangeHandler(
    (e) => {loadGeojsonFromFile(the_map, infoControl, loadFromFileButton._input); }
);

// button to load mocked data :
loadMockedButton.setClickHandler(
    (e) => {clearUploadedFile(loadFromFileButton._input); loadMockedGeojson(the_map, infoControl); }
);

// button to unload data :
unloadButton.setClickHandler(
    (e) => {clearUploadedFile(loadFromFileButton._input); unloadGeojson(the_map, infoControl);}
);

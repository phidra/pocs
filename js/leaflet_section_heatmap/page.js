import {initMap} from "./map.js";
import {initControls} from "./controls.js";
import {loadGeojsonFromFile, handleRequestedSectionId} from "./geojson.js";
import {loadStats, id2stats, id2rank} from "./stats.js";

const the_map = initMap();
const [infoControl, loadSectionsButton, loadStatsButton, requestSectionIdButton] = initControls(the_map);

// button to load section geometries :
loadSectionsButton.setChangeHandler(
    (e) => {loadGeojsonFromFile(the_map, infoControl, loadSectionsButton._input, id2stats, id2rank); }
);

// button to load section stats :
loadStatsButton.setChangeHandler(
    (e) => {loadStats(the_map, infoControl, loadStatsButton._input);}
);

// button to request an id :
requestSectionIdButton.setClickHandler(
    (e) => {
        const section_id = requestSectionIdButton.getRequestedSectionId();
        handleRequestedSectionId(the_map, section_id, infoControl, id2stats, id2rank);
    }
);

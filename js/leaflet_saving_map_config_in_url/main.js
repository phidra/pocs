import {initMap} from "./map.js";
import {initControl} from "./control.js";
import {initMarker, setChangedHandler} from "./markers.js";
import {parse_url_params, update_url} from "./url.js";

document.addEventListener("DOMContentLoaded", function() {
    // parsing params :
    const [center, zoom, markerpos] = parse_url_params();
    console.log(`using map zoom ${zoom}`);
    console.log(`using map center ${center}`);
    console.log(`using makerpos ${markerpos}`);

    const the_map = initMap(center, zoom);
    const marker = initMarker(the_map, markerpos);
    const statusControl = initControl(the_map, marker);

    // update URL (and status) when marker is dragged :
    function marker_changed() {
        statusControl.update(marker);
        let lat = marker.getLatLng().lat.toFixed(6);
        let lng = marker.getLatLng().lng.toFixed(6);
        update_url({
            markerpos: [lat, lng],
        });

    };
    setChangedHandler(marker_changed);

    // update URL when map is moved :
    function map_changed() {
        let lat = the_map.getCenter().lat.toFixed(6);
        let lng = the_map.getCenter().lng.toFixed(6);
        update_url({
            mapcenter: [lat, lng],
            mapzoom: the_map.getZoom(),
        });
	};
    the_map.on("moveend", map_changed);

    map_changed();
    marker_changed();
});

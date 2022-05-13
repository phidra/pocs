let marker = null;

export function setChangedHandler(handler) {
    marker.on("dragend", handler);
}

export function initMarker(the_map, markerpos) {
    if (markerpos === null || markerpos === undefined) { markerpos = the_map.getCenter(); }
    marker = L.marker(markerpos, { draggable: true});
    marker.addTo(the_map);
    return marker;
}

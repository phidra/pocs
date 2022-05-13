const URL_TEMPLATE = "https://a.tile.openstreetmap.org/{z}/{x}/{y}.png";
const MAX_ZOOM = 19;

export function initMap(center, zoom) {
    const the_map = L.map('the_map');
    L.tileLayer(URL_TEMPLATE, {maxZoom: MAX_ZOOM}).addTo(the_map);
    the_map.setView(center, zoom);
    return the_map;
};

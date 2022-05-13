export function initMap() {
    const the_map = L.map('mapid');
    the_map.zoomControl.remove();
    the_map.doubleClickZoom.disable();
    the_map.setView([48.8495, 2.3568], 15);
    const osm_url_template = "https://a.tile.openstreetmap.org/{z}/{x}/{y}.png";
    const url_template = osm_url_template;
    L.tileLayer(url_template, {
        maxZoom: 19,
    }).addTo(the_map);
    return the_map;
};

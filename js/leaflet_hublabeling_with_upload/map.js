export function initMap() {
    const the_map = L.map('mapid').setView([48.8534, 2.3488], 16);
    const osm_url_template = "https://a.tile.openstreetmap.org/{z}/{x}/{y}.png";
    const url_template = osm_url_template;
    L.tileLayer(url_template, {
        maxZoom: 19,
    }).addTo(the_map);
    return the_map;
};

L.Control.Status = L.Control.extend({
    onAdd: function(map) {
        this._div = L.DomUtil.create('div', 'panel');
        return this._div;
    },

    update: function(marker) {
        const latitude = marker.getLatLng().lat;
        const longitude = marker.getLatLng().lng;
        this._div.innerHTML = `<b>latitude=</b>${latitude}<br/><b>longitude=</b>${longitude}`;
    },
});

export function initControl(the_map, marker) {
    const statusControl = new L.Control.Status({ position: 'topright' });
    statusControl.addTo(the_map);
    statusControl.update(marker);
    return statusControl;
};


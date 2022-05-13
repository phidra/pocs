const regular = {
    radius: 12,
    stroke: true,
    color: "grey",
    fill: true,
    fillOpacity: 0.5,
    fillColor: "grey",
};

const hovered = {
    radius: 15,
    color: "red",
    fillOpacity: 0.8,
    fillColor: "red",
};

const inhub = {
    radius: 12,
    color: "green",
    fillOpacity: 0.65,
    fillColor: "green",
};

const outhub = {
    radius: 12,
    color: "blue",
    fillOpacity: 0.65,
    fillColor: "blue",
};

function pointToLayer(geoJsonPoint, latlng) {
    return L.circleMarker(latlng, regular);
}

export {
    regular,
    hovered,
    inhub,
    outhub,
    pointToLayer,
};

import * as styles from "./styles.js";
import {mocked_data} from "./mocked_data.js";

var geojsonLayer = null;

function handleHover(e, infoControl) {
    // styling du point hovered :
    const hoveredLayer = e.target;
    hoveredLayer.setStyle(styles.hovered);

    // styling de ses hubs :
    const inhubs = hoveredLayer.feature.properties.inhubs;
    const outhubs = hoveredLayer.feature.properties.outhubs;
    geojsonLayer.eachLayer((layer) => {
        if (layer != hoveredLayer)
        {
            if (inhubs.includes(layer.feature.properties.id))
            {
                layer.setStyle(styles.inhub);
            }
            if (outhubs.includes(layer.feature.properties.id))
            {
                layer.setStyle(styles.outhub);
            }
        }
    });

    // mise à jour de l'infoControl :
    infoControl.update(hoveredLayer.feature.properties);
}

function resetHover(infoControl) {
    if (geojsonLayer !== null)
    {
        geojsonLayer.resetStyle();
        geojsonLayer.eachLayer((layer) => {layer.setStyle(styles.regular);});
    }
    infoControl.reset();
}

export function loadGeojsonFromFile(the_map, infoControl, fileUploadInput) {
    var files = fileUploadInput.files;
    if (files.length == 0) { return; }  // do nothing if no file
    const file = files[0];
    alert("Reading from file : '" + file.name + "'\nsize = " + file.size + " bytes\ntype = '" + file.type + "'");

    const reader = new FileReader();
    reader.onload = function() {
        if (reader.readyState != 2 || reader.error){ return; }
        const content = JSON.parse(reader.result);
        _loadGeojson(the_map, infoControl, content);
    }
    reader.readAsText(file);
};

export function loadMockedGeojson(the_map, infoControl) {
    return _loadGeojson(the_map, infoControl, mocked_data);
};

function _loadGeojson(the_map, infoControl, geojsonData) {

    if (geojsonData !== null) { unloadGeojson(the_map, infoControl); }

    // The onEachFeature option is a function that gets called on each feature before adding it to a GeoJSON layer
    function onEachFeature(feature, layer) {
        // note : on dirait que c'est bien sur le layer que l'event est attaché (et pas sur le point lui-même)
        //        du coup, ce n'est que dans le handler qu'on récupère la feature à laquelle s'applique l'event.
        //        Ou bien j'ai mal compris, et ici "layer" est l'objet leaflet représentant la feature geojson ?
        //        EDIT : a priori, c'est ça : layer contient l'objet leaflet (le point, quoi) et non l'ensemble des objets.
        layer.on({
            mouseover: (e) => handleHover(e, infoControl),
            mouseout: (e) => resetHover(infoControl),
        });
    }

    const geojsonOptions = {
        pointToLayer: styles.pointToLayer,
        onEachFeature: onEachFeature,
    };

    geojsonLayer = L.geoJson(geojsonData, geojsonOptions).addTo(the_map);
    the_map.fitBounds(geojsonLayer.getBounds());
};

export function clearUploadedFile(fileUploadInput) {
    fileUploadInput.value = "";
};

export function unloadGeojson(the_map, infoControl) {
    resetHover(infoControl);
    if (geojsonLayer !== null && the_map.hasLayer(geojsonLayer))
    {
        the_map.removeLayer(geojsonLayer);
    }
    geojsonLayer = null;
};

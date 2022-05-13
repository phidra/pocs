// POC fortement inspirée de : https://leafletjs.com/examples/choropleth/
import * as styles from "./styles.js";

var geojson;

function handleHover(e, info) {
    // styling du point hovered :
    const hoveredLayer = e.target;
    hoveredLayer.setStyle(styles.hovered);

    // styling de ses hubs :
    const inhubs = hoveredLayer.feature.properties.inhubs;
    const outhubs = hoveredLayer.feature.properties.outhubs;
    geojson.eachLayer((layer) => {
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

    // mise à jour de l'info :
    info.update(hoveredLayer.feature.properties);
}

function resetHover(e, info) {
    geojson.resetStyle();
    geojson.eachLayer((layer) => {layer.setStyle(styles.regular);});
    info.reset();
}

export function displayGeojson(the_map, info, geojsonData) {

    // The onEachFeature option is a function that gets called on each feature before adding it to a GeoJSON layer
    function onEachFeature(feature, layer) {
        // note : on dirait que c'est bien sur le layer que l'event est attaché (et pas sur le point lui-même)
        //        du coup, ce n'est que dans le handler qu'on récupère la feature à laquelle s'applique l'event.
        //        Ou bien j'ai mal compris, et ici "layer" est l'objet leaflet représentant la feature geojson ?
        //        EDIT : a priori, c'est ça : layer contient l'objet leaflet (le point, quoi) et non l'ensemble des objets.
        layer.on({
            mouseover: (e) => handleHover(e, info),
            mouseout: (e) => resetHover(e, info),
        });
    }

    const geojsonOptions = {
        pointToLayer: styles.pointToLayer,
        onEachFeature: onEachFeature,
    };

    geojson = L.geoJson(geojsonData, geojsonOptions).addTo(the_map);
    the_map.fitBounds(geojson.getBounds());
};

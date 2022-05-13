var geojsonLayer = null;

const defaultStyle = {
	"color": "red",
	"weight": 8,
};

const selectedStyle = {
	"color": "blue",
	"weight": 12,
};

function handleClick(e, infoControl, id2stats, id2rank) {
    const layer = e.target;
    const section_id = layer.feature.properties.section_id;
    console.log("cliked section_id = " + section_id);
    if (section_id === undefined) {
        alert("ERROR> undefined 'section_id' on clicked polyline");
        return
    }

    if (section_id === infoControl.selected_section_id) {
        unselect(infoControl);
    }
    else {
        unselect(infoControl);
        selectSection(layer, section_id, infoControl, id2stats, id2rank);
    }
}

export function handleRequestedSectionId(the_map, section_id, infoControl, id2stats, id2rank) {
    if (geojsonLayer === null)
    {
        alert("no sections !");
        return;
    }

    // find layer matching requested section_id :
    var nb_found_layers = 0;
    let found_layer = null;
    unselect(infoControl);
    geojsonLayer.eachLayer((layer) => {
        const layer_section_id = layer.feature.properties.section_id;
        if (layer_section_id === section_id) {
            nb_found_layers += 1;
            found_layer = layer;
        }
    });

    selectSection(found_layer, section_id, infoControl, id2stats, id2rank);

    if (nb_found_layers == 0) {
        alert("unable to found geometry of section_id '" + section_id + "'");
    }
    else if (nb_found_layers > 1) {
        console.log("WARNING : found more than 1 (=" + nb_found_layers + ") layers with section_id '" + section_id + "'");
    }
    else {
        the_map.fitBounds(found_layer.getBounds());
    }
}

function selectSection(layer, section_id, infoControl, id2stats, id2rank) {
    if (layer !== undefined && layer !== null) {
        layer.setStyle(selectedStyle);
        layer.bringToFront();
    }
    else {
        console.log("no geometry for this section");
    }
    if (! id2stats.hasOwnProperty(section_id)) {
        alert("ERROR> no stats for clicked section_id '" + section_id + "' -> did you forgot to load stats ?");
        return;
    }

    const section_stats = id2stats[section_id];
    const section_rank = id2rank[section_id];
    const maxrank = id2rank.maxrank;
    infoControl.update(section_id, section_stats.count, section_stats.ratio, section_rank, maxrank);
}

function unselect(infoControl) {
    if (geojsonLayer !== null)
    {
		geojsonLayer.eachLayer((layer) => {layer.setStyle(defaultStyle);});
    }
    infoControl.reset();
}

export function loadGeojsonFromFile(the_map, infoControl, fileUploadInput, id2stats, id2rank) {
    var files = fileUploadInput.files;
    if (files.length == 0) { return; }  // do nothing if no file
    const file = files[0];
    console.log("Reading from file : '" + file.name + "'\nsize = " + file.size + " bytes\ntype = '" + file.type + "'");

    const reader = new FileReader();
    reader.onload = function() {
        if (reader.readyState != 2 || reader.error){ return; }
        const content = JSON.parse(reader.result);


        function onEachFeature(feature, layer) {
            layer.on({
                click: (e) => handleClick(e, infoControl, id2stats, id2rank),
            });
        }

        const geojsonOptions = {
            onEachFeature: onEachFeature,
			style: defaultStyle,
        };


        geojsonLayer = L.geoJson(content, geojsonOptions).addTo(the_map);
    }
    reader.readAsText(file);
};

L.Control.SectionInfo = L.Control.extend({
    onAdd: function(map) {
        this._div = L.DomUtil.create('div', 'info');
        this.reset();
        return this._div;
    },

    update: function(id, count, ratio, rank, maxrank) {
        this.selected_section_id = id;
        this._div.innerHTML = (
            '<b>section : ' + id  + '</b> <br/>' +
            '<b>count: </b>' + count + '<br/>' +
            '<b>ratio: </b>' + ratio + '<br/>' +
            '<b>rank: </b>' + rank + ' of ' + maxrank + '<br/>' +
            '<br/>' +
            "<b>ratio</b> = cette section apparaissait dans X% des itis<br/>" +
            "<b>rank</b> = cette section est la N-ième plus importante (1=très important)<br/>" +
            '<br/>' +
            '<b>AIDE</b> : uploadez <span style="color: red;">un geojson de sections</span>, <span style="color: blue;">un json de statistiques</span> <br/>' +
            'puis cliquez sur une section pour voir ses stats. <br/>' +
            'Des fichier samples sont dans le même répertoire que le code <br/>'
        );
    },

    reset: function() {
        this.selected_section_id = null;
        this._div.innerHTML = (
            '<b>section : undefined </b> <br/>' +
            '<b>count: </b> undefined <br/>' +
            '<b>ratio: </b> undefined <br/>' +
            '<b>rank: </b> undefined <br/>' +
            '<br/>' +
            "<b>ratio</b> = cette section apparaissait dans X% des itis<br/>" +
            "<b>rank</b> = cette section est la N-ième plus importante (1=très important)<br/>" +
            '<br/>' +
            '<b>AIDE</b> : uploadez <span style="color: red;">un geojson de sections</span>, <span style="color: blue;">un json de statistiques</span> <br/>' +
            'puis cliquez sur une section pour voir ses stats. <br/>' +
            'Des fichier samples sont dans le même répertoire que le code <br/>'
        );
    },

    onRemove: function(map) {
    },

    selected_section_id: null,
});

L.control.sectioninfo = function(opts) {
    return new L.Control.SectionInfo(opts);
}

L.Control.LoadStatsButton = L.Control.extend({
    onAdd: function(map) {
        this._input = L.DomUtil.create('input', 'loadstatsbutton');
        this._input.setAttribute("type", "file");
        return this._input;
    },

    setChangeHandler: function(handler) {
        this._input.onchange = handler;
    },

    onRemove: function(map) {
    }
});

L.control.loadstatsbutton = function(opts) {
    return new L.Control.LoadStatsButton(opts);
}

L.Control.LoadSectionsButton = L.Control.extend({
    onAdd: function(map) {
        this._input = L.DomUtil.create('input', 'loadsectionsbutton');
        this._input.setAttribute("type", "file");
        return this._input;
    },

    setChangeHandler: function(handler) {
        this._input.onchange = handler;
    },

    onRemove: function(map) {
    }
});

L.control.loadsectionsbutton = function(opts) {
    return new L.Control.LoadSectionsButton(opts);
}

L.Control.RequestSectionIdButton = L.Control.extend({
    onAdd: function(map) {
        this._div = L.DomUtil.create("div", "info");
        this._textInput = L.DomUtil.create("input", "request-id-input", this._div);
        this._textInput.setAttribute("type", "number");
        this._textInput.setAttribute("placeholder", "e.g. 12500001189936");
        this._button = L.DomUtil.create("button", "request-id-button", this._div);
        this._button.innerHTML = "GO !";
        return this._div;
    },

    getRequestedSectionId() {
        return parseInt(this._textInput.value);
    },

    setClickHandler(handler) {
        this._button.onclick = handler;
    },

    onRemove: function(map) {
    }
});

L.control.requestsectionidbutton = function(opts) {
    return new L.Control.RequestSectionIdButton(opts);
}

export function initControls(the_map) {
    const infoControl = L.control.sectioninfo({ position: 'topright' }).addTo(the_map);
    const loadSectionsButton = L.control.loadsectionsbutton({ position: 'topright' }).addTo(the_map);
    const loadStatsButton = L.control.loadstatsbutton({ position: 'topright' }).addTo(the_map);
    const requestSectionIdButton = L.control.requestsectionidbutton({ position: 'topleft' }).addTo(the_map);
    return [infoControl, loadSectionsButton, loadStatsButton, requestSectionIdButton];
};

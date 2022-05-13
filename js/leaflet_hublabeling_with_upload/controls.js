L.Control.NodeInfo = L.Control.extend({
    onAdd: function(map) {
        this._div = L.DomUtil.create('div', 'info');
        this.reset();
        return this._div;
    },

    update: function(props) {
        // precondition : props is defined
        this._div.innerHTML = (
            '<h4>Node : ' + props.id +  '</h4>' +
            '<b style="color: green;">inhubs: </b>' + props.inhubs.join(",") + '<br /><b style="color: blue;">outhubs: </b>' + props.outhubs.join(",")
        );
    },

    reset: function() {
        this._div.innerHTML = (
            '<h4>Node : undefined </h4>' +
            'Hover over a point'
        );
    },

    onRemove: function(map) {
    }
});

L.control.nodeinfo = function(opts) {
    return new L.Control.NodeInfo(opts);
}

L.Control.LoadMockedButton = L.Control.extend({
    onAdd: function(map) {
        this._button = L.DomUtil.create('button', 'loadmockedbutton');
        this._button.innerHTML = "Load mocked data";
        return this._button;
    },

    setClickHandler: function(handler) {
        this._button.onclick = handler;
    },

    onRemove: function(map) {
    }
});

L.control.loadmockedbutton = function(opts) {
    return new L.Control.LoadMockedButton(opts);
}

L.Control.LoadFromFileButton = L.Control.extend({
    onAdd: function(map) {
        this._input = L.DomUtil.create('input', 'loadfromfilebutton');
        this._input.setAttribute("id", "uploadedfile");
        this._input.setAttribute("type", "file");
        return this._input;
    },

    setChangeHandler: function(handler) {
        this._input.onchange = handler;
    },

    onRemove: function(map) {
    }
});

L.control.loadfromfilebutton = function(opts) {
    return new L.Control.LoadFromFileButton(opts);
}

L.Control.UnloadButton = L.Control.extend({
    onAdd: function(map) {
        this._button = L.DomUtil.create('button', 'unloadbutton');
        this._button.innerHTML = "Unload data";
        return this._button;
    },

    setClickHandler: function(handler) {
        this._button.onclick = handler;
    },

    onRemove: function(map) {
    }
});

L.control.unloadbutton = function(opts) {
    return new L.Control.UnloadButton(opts);
}

export function initControls(the_map) {
    const infoControl = L.control.nodeinfo({ position: 'topright' }).addTo(the_map);
    const loadMockedButton = L.control.loadmockedbutton({ position: 'topright' }).addTo(the_map);
    const loadFromFileButton = L.control.loadfromfilebutton({ position: 'topright' }).addTo(the_map);
    const unloadButton = L.control.unloadbutton({ position: 'topright' }).addTo(the_map);
    return [infoControl, loadMockedButton, loadFromFileButton, unloadButton];
};

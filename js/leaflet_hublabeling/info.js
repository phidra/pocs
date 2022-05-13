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

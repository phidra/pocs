export var id2stats = {};
export var id2rank = {};

function fillStructures(stats) {
    stats.sort((a, b) => b["ratio"] - a["ratio"]);
    stats.forEach(function(stat, rank) {
        const section_id = stat["id"];
        id2stats[section_id] = stat;
        id2rank[section_id] = rank;

        if (rank < 5) {
            console.log("[rank=" + rank + "] " + section_id + "   (count=" + stat["count"] + "  /  ratio=" + stat["ratio"] + ")");
        }
        else if (rank > (stats.length - 5)) {
            console.log("[rank=" + rank + "] " + section_id + "   (count=" + stat["count"] + "  /  ratio=" + stat["ratio"] + ")");
        }
    });
    id2rank["maxrank"] = stats.length;
};

export function loadStats(the_map, infoControl, fileUploadInput) {
    var files = fileUploadInput.files;
    if (files.length == 0) { return; }  // do nothing if no file
    const file = files[0];
    console.log("Reading from file : '" + file.name + "'\nsize = " + file.size + " bytes\ntype = '" + file.type + "'");

    const reader = new FileReader();
    reader.onload = function() {
        if (reader.readyState != 2 || reader.error){ return; }
        let content = JSON.parse(reader.result);
        fillStructures(content["sections"]);
    }
    reader.readAsText(file);
};

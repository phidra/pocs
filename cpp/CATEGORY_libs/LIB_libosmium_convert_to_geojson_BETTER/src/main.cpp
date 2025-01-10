#include <fstream>
#include <iomanip>
#include <iostream>

#include "common/graphtypes.h"
#include "dumping/geojson.h"
#include "graph/graph.h"
#include "parsing/polygonfile.h"

using namespace std;

void usage(char const* prog_name) {
    const std::string description = R"DELIM(
Convert a given OSM file to a geojson.
Inputs :
    OSMFILE   : an OSM extract which will be used as a walk-graph
    POLYGONFILE   : path to a geojson file storing a polygon to filter OSM ways.
    OUTPUT-DIR : directory to store the geojson file.

)DELIM";
    cerr << description << "\n";
    cerr << "Usage: \n";
    cerr << "\t" << prog_name << " OSMFILE  POLYGONFILE  OUTPUT-DIR\n";
    cerr << "For instance : \n";
    cerr << "\t" << prog_name << " DATA/monaco.osm.pbf  DATA/polygon.geojson  /path/to/output_dir\n";

    ostringstream oss;
    oss << "Use special value '" << NO_POLYGON << "' as POLYGONFILE to disable the filtering by a polygon.";
    cerr << oss.str() << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        usage(argv[0]);
        exit(1);
    }

    try {
        auto osmfile = argv[1];
        auto polygonfile = argv[2];
        string output_dir(argv[3]);
        if (output_dir.back() != '/') {
            output_dir.push_back('/');
        }

        cerr << "OSM FILE       : " << osmfile << endl;
        cerr << "POLYGON FILE   : " << polygonfile << endl;
        cerr << "DUMPING TO     : " << output_dir << endl;

        // convert osmfile to graph :
        BgPolygon polygon;
        try {
            polygon = get_polygon(polygonfile);
        } catch (runtime_error& e) {
            cerr << "ERROR : " << e.what() << "\n" << endl;
            usage(argv[0]);
            exit(2);
        }
        vector<Edge> edges = osm_to_graph(osmfile, polygon);

        // dumps :
        ofstream geojson_graph(output_dir + "graph.geojson");
        dump_geojson_graph(geojson_graph, edges);

    } catch (exception const& e) {
        cerr << e.what() << '\n';
        exit(1);
    }

    cerr << "All is ok." << endl;
    return 0;
}

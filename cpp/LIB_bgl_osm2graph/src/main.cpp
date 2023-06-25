#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

#include "graphtypes.h"
#include "graph.h"
#include "dump_to_geojson.h"
#include "parse_polygonfile.h"

using namespace std;

void usage(const char* prog_name) {
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
        auto returned = osm_to_graph(osmfile, polygon);
        auto edges = returned.first;
        auto boost_graph = returned.second;

        cout << "Vertex n°42 in the graph has these properties :" << endl;
        auto my_v = vertex(42, boost_graph);
        cout << setprecision(10);
        cout << "\tLAT = " << boost_graph[my_v].latitude << endl;
        cout << "\tLON = " << boost_graph[my_v].longitude << endl;
        cout << "\tID  = " << boost_graph[my_v].osmid << endl;
        cout << endl;

        // dumps :
        cout << "Dumping graphs" << endl;
        ofstream stream1(output_dir + "graph_edgesvector.geojson");
        dump_geojson_graph(stream1, edges);
        ofstream stream2(output_dir + "graph_boostgraph.geojson");
        dump_geojson_graph(stream2, boost_graph);



        // ensuring that both geojson are identical :
        ostringstream oss1;
        dump_geojson_graph(oss1, edges);
        ostringstream oss2;
        dump_geojson_graph(oss1, boost_graph);
        assert(oss1.str() == oss2.str());


    } catch (const exception& e) {
        cerr << e.what() << '\n';
        exit(1);
    }

    cerr << "All is ok." << endl;
    return 0;
}

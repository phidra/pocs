#include <routingkit/osm_simple.h>
#include <routingkit/vector_io.h>
#include <routingkit/timer.h>
#include <routingkit/contraction_hierarchy.h>
#include <routingkit/min_max.h>

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace RoutingKit;
using namespace std;

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            cout << "Usage:     " << argv[0] << "  <INPUT-OSMFILE>  <OUTPUT-SVGFILE>" << endl;
            cout << "Example:   " << argv[0] << "  NOGIT_monaco.osm.pbf  NOGIT_monaco.svg" << endl;
            exit(1);
        }

        const string osm_file = argv[1];
        const string svg_file = argv[2];
        cout << "INPUT  OSMFILE = " << osm_file << endl;
        cout << "OUTPUT SVGFILE = " << svg_file << endl;
        cout << endl;

        // with this bool to TRUE, all nodepairs will be drawn (including the ones inside an edge)
        // with this bool to FALSE, only the extremities of each edge will be drawn
        bool CONSIDER_THAT_MODELLING_NODES_ARE_ROUTING_NODES = true;
        auto graph = simple_load_osm_pedestrian_routing_graph_from_pbf(
            osm_file, [](string const&) {}, CONSIDER_THAT_MODELLING_NODES_ARE_ROUTING_NODES);

        // scaling all latitudes to [0 ; 300]
        float min_latitude = min_element_of(graph.latitude);
        float max_latitude = max_element_of(graph.latitude);
        for (auto& x : graph.latitude) {
            x -= min_latitude;
            x *= 300;
            x /= (max_latitude - min_latitude);
        }

        // scaling all longitudes to [0 ; 300]
        float min_longitude = min_element_of(graph.longitude);
        float max_longitude = max_element_of(graph.longitude);
        for (auto& x : graph.longitude) {
            x -= min_longitude;
            x *= 300;
            x /= (max_longitude - min_longitude);
        }

        cout << "Writing SVG ... " << flush;

        std::ofstream out(svg_file);
        if (!out)
            throw std::runtime_error("Can not open file " + svg_file);

        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0 0 "
               "300 300\">\n";
        for (unsigned x = 0; x < graph.first_out.size() - 1; ++x) {
            for (unsigned xy = graph.first_out[x]; xy < graph.first_out[x + 1]; ++xy) {
                unsigned y = graph.head[xy];

                out << "<line "
                    << "x1=\"" << graph.longitude[x] << "\" y1=\"" << 300 - graph.latitude[x] << "\" "
                    << "x2=\"" << graph.longitude[y] << "\" y2=\"" << 300 - graph.latitude[y]
                    << "\" style=\"stroke:#000000;\"/>\n";
            }
        }
        out << "</svg>\n";

        cout << "done" << endl;

    } catch (exception& err) {
        cerr << "Stopped on exception : " << err.what() << endl;
    }
}

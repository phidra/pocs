#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "connected_components.h"
#include "dump_to_geojson.h"
#include "graph.h"
#include "graphtypes.h"
#include "parse_polygonfile.h"

using namespace std;
namespace bg = boost::geometry;

void usage(char const* prog_name) {
    const string description = R"DELIM(
From a given OSM file (filtered by a geojson polygon), computes and dumps connected components.
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
        auto boost_graph = osm_to_graph(osmfile, polygon);

        cerr << "À ce stade, le graphe est construit, reste à appliquer un algo de la BGL dessus" << endl;

        // dumping graph :
        ofstream graph_stream(output_dir + "graph.geojson");
        dump_geojson_graph(graph_stream, boost_graph);

        // computing + dumping connected components :
        vector<vector<VertexDescriptor>> connected_vertices = compute_connected_vertices(boost_graph);
        constexpr int const size_display_threshold = 15;
        display_connected_vertices(boost_graph, connected_vertices, size_display_threshold);
        dump_connected_vertices_hull(boost_graph, connected_vertices, output_dir);

        // computing + dumping biconnected components :
        BiconnResults biconns = compute_biconnected_vertices(boost_graph);
        dump_biconnected_vertices_hull(boost_graph, biconns, output_dir);

        // dump des articulation points :
        ofstream art_points_stream(output_dir + "articulation_points.geojson");
        points_to_geojson_points(art_points_stream, biconns.art_points);

        // computing potential bridges = biconnected-components containing only a single edge :
        // amongst those bridges, we distinguish :
        //      - deadends = edge at the end of a road (one of the two nodes of the edge has no other incident edge than
        //      the bridge itself)
        //      - real bridges = potential bridges that are not deadends
        using BiconnAndEdge = decltype(biconns.biconn2edges)::value_type;
        vector<BiconnAndEdge> potential_bridges;
        copy_if(biconns.biconn2edges.cbegin(),
                biconns.biconn2edges.cend(),
                back_inserter(potential_bridges),
                [](auto const& x) {
                    auto biconn_id = x.first;
                    auto const& edges = x.second;
                    return edges.size() == 1;
                });
        cout << "Combien de biconns en tout ? " << biconns.biconn2edges.size() << endl;
        cout << "Combien de biconns à un seul edge ? " << potential_bridges.size() << endl;

        // identifying dead-ends = potential bridges for which a node only has 1 edge :
        set<EdgeDescriptor> deadend_bridges;
        set<EdgeDescriptor> real_bridges;
        for (auto x : potential_bridges) {
            auto biconn_id = x.first;
            auto edge = *x.second.begin();
            VertexDescriptor node_from = source(edge, boost_graph);
            VertexDescriptor node_to = target(edge, boost_graph);
            auto out_degree_from = out_degree(node_from, boost_graph);
            auto out_degree_to = out_degree(node_to, boost_graph);
            bool is_deadend = out_degree_from == 1 || out_degree_to == 1;
            if (is_deadend) {
                deadend_bridges.insert(edge);
            } else {
                real_bridges.insert(edge);
            }
        }
        cout << "Combien de deadend_bridges ? " << deadend_bridges.size() << endl;
        cout << "Combien de real_bridges ? " << real_bridges.size() << endl;
        ofstream deadends_stream(output_dir + "deadends.geojson");
        edges_to_geojson_linestrings(deadends_stream, deadend_bridges, boost_graph, "#ff0000", 5);

        // note : unmerged = two adjacent bridges will not be merged :
        ofstream unmerged_bridges_stream(output_dir + "unmerged_bridges.geojson");
        edges_to_geojson_linestrings(unmerged_bridges_stream, real_bridges, boost_graph, "#0000ff", 5);

        // here, we want to merge adjacent bridges (we want to treat multiple consecutive bridges as a single big
        // bridge) :
        vector<Polyline> merged_bridges;  // we will mutate elements -> set is not usable
        for (auto x : potential_bridges) {
            auto biconn_id = x.first;
            auto current_edge = *x.second.begin();
            auto current_edge_geometry = boost_graph[current_edge].geometry;
            VertexDescriptor node_from = source(current_edge, boost_graph);
            VertexDescriptor node_to = target(current_edge, boost_graph);
            auto out_degree_from = out_degree(node_from, boost_graph);
            auto out_degree_to = out_degree(node_to, boost_graph);
            bool is_deadend = out_degree_from == 1 || out_degree_to == 1;
            if (!is_deadend) {
                // searching for a adjacent bridge :
                auto current_edge_extremity1 = current_edge_geometry.front();
                auto current_edge_extremity2 = current_edge_geometry.back();
                bool is_edge_adjacent_to_an_existing_bridge = false;

                // this way of merging is not optimal (quadratic), but it'll do the work for now...
                for (auto& existing_bridge_geometry : merged_bridges) {
                    auto bridge_extremity1 = existing_bridge_geometry.front();
                    auto bridge_extremity2 = existing_bridge_geometry.back();

                    // if both bridges are adjacent, we'll go in one of these branches, and merge geometries :
                    if (current_edge_extremity1 == bridge_extremity1) {
                        // note : utiliser swap
                        current_edge_geometry.insert(current_edge_geometry.end(),
                                                     make_move_iterator(++existing_bridge_geometry.rbegin()),
                                                     make_move_iterator(existing_bridge_geometry.rend()));
                        existing_bridge_geometry = current_edge_geometry;
                        is_edge_adjacent_to_an_existing_bridge = true;
                        break;
                    } else if (current_edge_extremity1 == bridge_extremity2) {
                        existing_bridge_geometry.insert(existing_bridge_geometry.end(),
                                                        make_move_iterator(++current_edge_geometry.begin()),
                                                        make_move_iterator(current_edge_geometry.end()));
                        is_edge_adjacent_to_an_existing_bridge = true;
                        break;
                    } else if (current_edge_extremity2 == bridge_extremity1) {
                        current_edge_geometry.insert(current_edge_geometry.end(),
                                                     make_move_iterator(++existing_bridge_geometry.begin()),
                                                     make_move_iterator(existing_bridge_geometry.end()));
                        existing_bridge_geometry = current_edge_geometry;
                        is_edge_adjacent_to_an_existing_bridge = true;
                        break;
                    } else if (current_edge_extremity2 == bridge_extremity2) {
                        existing_bridge_geometry.insert(existing_bridge_geometry.end(),
                                                        make_move_iterator(++current_edge_geometry.rbegin()),
                                                        make_move_iterator(current_edge_geometry.rend()));
                        is_edge_adjacent_to_an_existing_bridge = true;
                        break;
                    }

                    // éventuellement, par robustesse, comparer des distances, plutôt ?
                    // s'il n'y a pas recouvremernt -> insérer l'edge en tant que nouveau bridge
                    // s'il y a recouvremernt -> merger l'edge avec le bridge existant
                }

                if (!is_edge_adjacent_to_an_existing_bridge) {
                    // new bridge is totally independant from existing bridges -> we add it :
                    merged_bridges.push_back(current_edge_geometry);
                } else {
                    // cout << "MERGE !" << endl;
                }
            }
        }

        cout << "Combien de merged_bridges ? " << merged_bridges.size() << endl;
        ofstream merged_bridges_stream(output_dir + "merged_bridges.geojson");
        geometries_to_geojson_linestrings(merged_bridges_stream, merged_bridges, "#0000ff", 5);

    } catch (exception const& e) {
        cerr << e.what() << '\n';
        exit(1);
    }

    cerr << "All is ok." << endl;
    return 0;
}

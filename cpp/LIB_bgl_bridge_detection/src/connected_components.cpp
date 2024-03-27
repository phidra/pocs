#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/biconnected_components.hpp>

#include "connected_components.h"
#include "convexhull.h"
#include "dump_to_geojson.h"

using namespace std;

vector<vector<VertexDescriptor>> compute_connected_vertices(Graph const& graph) {
    // computing connected components :
    vector<int> components(boost::num_vertices(graph));
    int num_cc = connected_components(graph, &components[0]);

    // dispatching vertex into connected components :
    vector<vector<VertexDescriptor>> connected_vertices(num_cc, vector<VertexDescriptor>{});
    for (size_t i = 0; i != components.size(); ++i) {
        auto conn_comp_index = components[i];
        auto v = vertex(i, graph);
        connected_vertices[conn_comp_index].push_back(v);
    }

    sort(connected_vertices.begin(), connected_vertices.end(),
         [](vector<VertexDescriptor> const& left, vector<VertexDescriptor> const& right) {
             return left.size() <= right.size();
         });

    return connected_vertices;
}

void display_connected_vertices(Graph const& graph,
                                vector<vector<VertexDescriptor>> const& connected_vertices,
                                const int size_display_threshold) {
    cout << "Displaying the " << connected_vertices.size() << " connected-components :" << endl;
    for (auto connected_component : connected_vertices) {
        cout << "This component has " << connected_component.size() << " vertices" << endl;
        if (connected_component.size() <= size_display_threshold) {
            cout << "\t(as it has less than " << size_display_threshold << " vertices, we display them)" << endl;
            for (auto vd : connected_component) {
                auto vertex = graph[vd];
                cout << setprecision(15);
                cout << "\t [" << vertex.latitude << ";" << vertex.longitude << "] -> " << node_url(vertex.osmid)
                     << endl;
            }
        }
    }
    cout << endl;
}

void dump_connected_vertices_hull(Graph const& graph,
                                  vector<vector<VertexDescriptor>> const& connected_vertices,
                                  string const& output_dir) {
    cout << "Dumping " << connected_vertices.size() << " connected components into output_dir : " << output_dir << endl;
    int cc_idx = 0;
    for (auto const& connected_component : connected_vertices) {
        // le nom du fichier dans lequel on dumpe la composante connexe suite ce modèle :
        //      OUTPUT-DIR/connected_component_<numéro_de_CC>_<nombre d'items>points.geojson

        auto results = build_convex_hull(graph, connected_component);
        auto points = results.first;
        auto hull = results.second;

        size_t nb_points = bg::num_points(points);
        ofstream cc_stream(output_dir + "HULL_connected_component_" + to_string(cc_idx++) + "_with" +
                           to_string(nb_points) + "points.geojson");

        if (nb_points > 2) {
            // la hull ne sert que si la composante connexe a au moins 3 vertex :
            points_to_geojson_linestring(cc_stream, hull.outer());
        } else if (nb_points == 2) {
            // avec seulement deux vertex dans la composante connexe, on se contente d'une ligne :
            points_to_geojson_linestring(cc_stream, points);
        } else {
            // on ignore la composante connexe s'il n'y a qu'un vertex dans la composante connexe :
            auto point = *boost::begin(points);
            double lat = bg::get<0>(point);
            double lng = bg::get<1>(point);
            cout << "This vertex is lonely, and will not appear in any geojson : [" << lng << ";" << lat << "]" << endl;
        }
    }
}

BiconnResults compute_biconnected_vertices(Graph const& graph) {
    BiconnResults results;

    // j'aurais bien aimé travailler avec edge_index_t pour définir la pmap plutôt comme un vector, mais je n'ai pas
    // réussi...
    map<EdgeDescriptor, int> edge2biconn;
    boost::associative_property_map<map<EdgeDescriptor, int>> edge2biconn_pmap(results.edge2biconn);

    auto returned =
        boost::biconnected_components(graph, edge2biconn_pmap, back_inserter(results.art_points_descriptors));

    results.nb_of_biconn_comps = returned.first;
    cout << "Il y a " << results.nb_of_biconn_comps << " biconnected-components" << endl;

    cout << "Taille de la map :" << results.edge2biconn.size() << endl;
    /* cout << "Ses premiers éléments : " << endl; */
    /* for (auto da_pair: results.edge2biconn) { */
    /*     EdgeDescriptor ed = da_pair.first; */
    /*     int biconn_id = da_pair.second; */
    /*     cout << "\t this edge has biconn_id : " << biconn_id << endl; */
    /* } */

    // transformation des articulation points :
    transform(results.art_points_descriptors.cbegin(), results.art_points_descriptors.cend(),
              back_inserter(results.art_points), [&graph](VertexDescriptor const& vd) {
                  return Point{graph[vd].latitude, graph[vd].longitude};
              });

    // ici, on dispatche chaque edge (ou plutôt ses vertex) dans un MultiPoint différent en fonction de la
    // biconnected-component. par ailleurs, on construit également une map associant le biconn_id aux edges qu'elle
    // contient :
    results.biconn_components_points.resize(results.nb_of_biconn_comps);
    for (auto da_pair : results.edge2biconn) {
        EdgeDescriptor ed = da_pair.first;
        size_t biconn_id = da_pair.second;
        results.biconn2edges[biconn_id].insert(ed);
        if (biconn_id >= results.biconn_components_points.size()) {
            cout << "ERROR (les biconn_ids ne commencent probablement pas à 0 mais à 1, du coup)" << endl;
            exit(2);
        }
        MultiPoint& this_biconn_comp_points = results.biconn_components_points[biconn_id];
        VertexDescriptor source_vd = boost::source(ed, graph);
        VertexDescriptor target_vd = boost::target(ed, graph);
        bg::append(this_biconn_comp_points, Point(graph[source_vd].latitude, graph[source_vd].longitude));
        bg::append(this_biconn_comp_points, Point(graph[target_vd].latitude, graph[target_vd].longitude));
    }
    // à ce stade, les edges chaque biconnected-component (ou plutôt leurs vertex) ont été ajoutés à un MultiPoint.
    // il y a autant de MultiPoint différent que de biconnected-component.

    return results;
}

void dump_biconnected_vertices_hull(Graph const& graph, BiconnResults const& biconns, string const& output_dir) {
    int bicc_idx = 0;
    for (auto points : biconns.biconn_components_points) {
        Polygon hull;
        bg::convex_hull(points, hull);

        size_t nb_points = bg::num_points(points);
        const string bicc_filename = output_dir + "HULL_biconnected_component_" + to_string(bicc_idx++) + "_" +
                                     to_string(nb_points) + "points.geojson";

        if (nb_points > 2) {
            // la hull ne sert que si la composante connexe a au moins 3 vertex :
            ofstream bicc_stream(bicc_filename);
            points_to_geojson_linestring(bicc_stream, hull.outer());
        } else if (nb_points == 2) {
            /* // avec seulement deux vertex dans la composante connexe, on se contente d'une ligne : */
            /* ofstream bicc_stream(bicc_filename); */
            /* points_to_geojson_linestring(bicc_stream, points); */
        } else {
            // on ignore la composante connexe s'il n'y a qu'un vertex dans la composante connexe :
            auto point = *boost::begin(points);
            double lat = bg::get<0>(point);
            double lng = bg::get<1>(point);
            cout << "This vertex is lonely, and will not appear in any geojson : [" << lng << ";" << lat << "]" << endl;
        }
    }
}

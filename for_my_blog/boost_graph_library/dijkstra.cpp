#include <iostream>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;
using namespace std;

int main(int, char*[]) {
    //========================================
    // STEP 1 = définition du graphe :

    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/bundles.html
    struct VertexProperty {
        string name;
        double latitude;
        double longitude;
    };
    struct EdgeProperty {
        int weight;
        string roadname;
    };

    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/adjacency_list.html
    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/using_adjacency_list.html
    using Graph = adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty>;
    Graph mygraph;

    // on manipule les vertex via un vertex_descriptor (resp. edge) :
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;
    using EdgeDescriptor = graph_traits<Graph>::edge_descriptor;

    // idem pour itérer :
    using VertexIterator = graph_traits<Graph>::vertex_iterator;
    using EdgeIterator = graph_traits<Graph>::edge_iterator;
    using OutEdgeIterator = graph_traits<Graph>::out_edge_iterator;

    //========================================
    // STEP 2 = création des vertex et edges :

    // Le graphe construit est celui-ci :
    //            10
    //      ---------------
    //     /               \
    //    /   5         8   \
    //   A---------B---------C
    //   |         |         |
    //   |1        |3        |5
    //   |         |         |
    //   D---------E         F
    //        9    |         |
    //             |5        |2
    //             |         |
    //             G---------H
    //                  3

    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/MutableGraph.html
    VertexDescriptor A = add_vertex({"A", 48.8472743, 2.3385463}, mygraph);
    VertexDescriptor B = add_vertex({"B", 48.8538388, 2.2667097}, mygraph);
    VertexDescriptor C = add_vertex({"C", 48.8822442, 2.3355303}, mygraph);
    VertexDescriptor D = add_vertex({"D", 48.8784585, 2.3682707}, mygraph);
    VertexDescriptor E = add_vertex({"E", 48.8783875, 2.2781327}, mygraph);
    VertexDescriptor F = add_vertex({"F", 48.8751455, 2.3496791}, mygraph);
    VertexDescriptor G = add_vertex({"G", 48.8695761, 2.3778204}, mygraph);
    VertexDescriptor H = add_vertex({"HHHHH", 999, 888}, mygraph);  // ces properties sont corrigées plus bas

    add_edge(A, B, {5, "Boulevard Saint-Michel"}, mygraph);
    add_edge(A, C, {10, "Avenue Mozart"}, mygraph);
    add_edge(A, D, {1, "Place Pigalle"}, mygraph);
    add_edge(B, C, {8, "Boulevard de la Vilette"}, mygraph);
    add_edge(B, E, {3, "Avenue de Neuilly"}, mygraph);
    add_edge(C, F, {5, "Rue de Paradis"}, mygraph);
    add_edge(D, E, {9, "Boulevard de Belleville"}, mygraph);
    add_edge(E, G, {5, "Rue Lecourbe"}, mygraph);
    add_edge(F, H, {2, "Avenue des Champs-Élysées"}, mygraph);
    add_edge(G, H, {3333, "Rue de la Paiiiiix"}, mygraph);  // ces properties sont corrigées plus bas

    //========================================
    // STEP 3 = consultation / modification du graphe, des vertex/edge et de leurs propriétés :

    assert(num_edges(mygraph) == 10);
    assert(num_vertices(mygraph) == 8);

    // on accède à un VertexProperty d'un VertexDescriptor donné via l'operator[] du Graph :
    string A_name = mygraph[A].name;
    assert(A_name == "A");

    // quelques fonctions pour se simplifier la vie :
    auto nameof = [&mygraph](VertexDescriptor const& vd) { return mygraph[vd].name; };

    auto display_vertex = [&mygraph](VertexDescriptor const& vd) {
        cout << "vertex " << mygraph[vd].name << " has coordinates (" << mygraph[vd].latitude << ";"
             << mygraph[vd].longitude << ")\n";
    };

    auto display_edge = [&mygraph, &nameof](EdgeDescriptor const& ed) {
        // on accède aux extrêmités d'un edge via source/target :
        VertexDescriptor from = source(ed, mygraph);
        VertexDescriptor to = target(ed, mygraph);
        cout << "edge [" << nameof(from) << "-" << nameof(to) << "] is " << mygraph[ed].roadname << " with weight "
             << mygraph[ed].weight << "\n";
    };

    // itérer sur les vertices + consulter les properties :
    VertexIterator v, v_end;
    for (tie(v, v_end) = vertices(mygraph); v != v_end; ++v) {
        // v est un VertexIterator
        // *v est un VertexDescriptor
        display_vertex(*v);
    }
    cout << endl;

    // même principe pour les edges :
    EdgeIterator e, e_end;
    for (tie(e, e_end) = edges(mygraph); e != e_end; ++e) {
        display_edge(*e);
    }
    cout << endl;

    // itérer sur les edges adjacents à un noeud donné :
    OutEdgeIterator o, o_end;
    for (tie(o, o_end) = out_edges(E, mygraph); o != o_end; ++o) {
        display_edge(*o);
    }
    cout << endl;

    // accès à un vertex/edge quelconque :
    VertexDescriptor E_bis = vertex(4, mygraph);    // vertex : via son index
    EdgeDescriptor GH = edge(G, H, mygraph).first;  // edge : via ses extrêmités

    // accéder aux noeuds d'un edge donné :
    VertexDescriptor node_from = source(GH, mygraph);
    VertexDescriptor node_to = target(GH, mygraph);
    assert(node_from == G && node_to == H);

    display_vertex(E_bis);
    display_edge(GH);
    cout << endl;

    // consultations diverses :
    string name_of_A = mygraph[A].name;
    assert(name_of_A == "A");

    double lat = mygraph[A].latitude;
    double lon = mygraph[A].longitude;
    cout << "vertex " << name_of_A << " has coordinates (" << lat << ";" << lon << ")\n";

    string name_of_road_GH = mygraph[GH].roadname;
    assert(name_of_road_GH == "Rue de la Paiiiiix");
    assert(mygraph[GH].weight == 3333);

    // modification des properties d'un vertex/edge :
    mygraph[H].name = "H";
    mygraph[H].latitude = 48.8404808;
    mygraph[H].longitude = 2.2935483;

    mygraph[GH].roadname = "Rue de la Paix";
    auto weight_property_map = get(&EdgeProperty::weight, mygraph);
    int weight_of_GH = get(weight_property_map, GH);
    assert(weight_of_GH == 3333);
    put(weight_property_map, GH, 3);
    assert(mygraph[GH].weight == 3);

    display_vertex(H);
    display_edge(GH);
    cout << endl;

    //========================================
    // STEP 4 = application du dijkstra + récupération des résultats :

    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/PropertyGraph.html
    auto vertex_index_pmap = get(vertex_index, mygraph);

    std::vector<VertexDescriptor> predecessors(num_vertices(mygraph));
    auto predecessors_pmap = make_iterator_property_map(predecessors.begin(), vertex_index_pmap);

    std::vector<int> tentative_distances(num_vertices(mygraph));
    auto tentative_distances_pmap = make_iterator_property_map(tentative_distances.begin(), vertex_index_pmap);

    auto SOURCE = A;
    auto TARGET = H;

    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/dijkstra_shortest_paths.html
    dijkstra_shortest_paths(
        mygraph, SOURCE,
        weight_map(weight_property_map).predecessor_map(predecessors_pmap).distance_map(tentative_distances_pmap));

    cout << "Le plus court chemin de " << nameof(SOURCE) << " vers " << nameof(TARGET)
         << " a pour poids total : " << get(tentative_distances_pmap, TARGET) << endl;

    // reconstruction (à rebours) du plus court chemin :
    vector<VertexDescriptor> shortest_path = {TARGET};
    VertexDescriptor current_predecessor = TARGET;
    auto next_predecessor = get(predecessors_pmap, current_predecessor);
    // lorsque le prédécesseur d'un vertex est lui-même, c'est qu'on est sur la source, ou sur un noeud injoignable
    // depuis la source cf. la doc de l'algo =
    // https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/dijkstra_shortest_paths.html
    while (next_predecessor != current_predecessor) {
        shortest_path.push_back(next_predecessor);
        current_predecessor = next_predecessor;
        next_predecessor = get(predecessors_pmap, current_predecessor);
    }

    // affichage du shortest path :
    cout << "Shortest-path from " << nameof(SOURCE) << " to " << nameof(TARGET) << " is :\n";
    for (auto vertex_i = shortest_path.rbegin(); vertex_i != shortest_path.rend(); ++vertex_i) {
        cout << nameof(*vertex_i);
    }
    cout << endl;

    return 0;
}

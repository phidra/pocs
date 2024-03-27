#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <numeric>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <cassert>

using namespace boost;
using namespace std;

int main(int, char*[]) {
    cout << endl;
    cout << "Cette POC vise à mieux comprendre la construction du graphe, elle se contente de " << endl;
    cout << "   - définir les edges du futur graphe et leurs properties" << endl;
    cout << "   - construire le graphe en utilisant les edges+properties" << endl;
    cout << "   - setterttribuer a posteriori les properties des vertex du graphe" << endl;
    cout << "   - vérifier que tout est cohérent" << endl;
    cout << endl;
    cout << "Ce que cette POC démontre :" << endl;
    cout << "   - l'utilisateur du même constructor que dans la POC 3 (permettant de construire le graphe à partir de "
            "ses edges)"
         << endl;
    cout
        << "   - le fait de setter a posteriori les properties des vertex (en les récupérant à partir de leurs indices)"
        << endl;
    cout << "   - divers accès aux edges/vertex, et leurs properties" << endl;
    cout << endl;

    // Utilisation des bundle-properties (cf. https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/bundles.html) :
    struct VertexProperty {
        string name;
        int altitude;
        long long tomtom_id;
    };
    struct EdgeProperty {
        int weight;
        long long tomtom_id;
        string name;
    };

    // définition de mon graphe (avec propriété sur les vertices et les edges) :
    using Graph = adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty>;
    using EdgeIterator = graph_traits<Graph>::edge_iterator;

    // On va modéliser le graphe suivant, comportant 8 vertex et 10 edges :
    //           10
    //         -------
    //        /       \
    //       / 5     8 \
    //      A-----B-----C
    //      |     |     |
    //      |1    |3    |5
    //      |     |     |
    //      D-----E     F
    //         9  |     |
    //            |5    |2
    //            |     |
    //            G-----H
    //               3

    // Tout comme dns la POC 3, on va utiliser le 6ième constructor de la section "Member functions" de cette page :
    //      https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/adjacency_list.html
    // Il permet de construire un graphe avec ses edges (munis de leur edge-properties), des vertices (éventuellement
    // vides). En revanche, on dirait qu'il ne permet pas de définir les properties des vertex.
    // TODO : un contournement que j'envisage (à tester dans une POC future) serait de définir les VertexProperties
    // comme external-properties ? (c'est pas fi-fou, notamment car les vertex-properties ont vocation à être internes)
    //
    // NOTE : voir aussi ceci : https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/IteratorConstructibleGraph.html

    // à ce stade, les vertices sont identifiés par leur index (c'est ce qu'attend le constructeur)
    vector<pair<int, int> > the_edges = {
        pair<int, int>{0, 1},  // AB
        pair<int, int>{0, 2},  // AC
        pair<int, int>{0, 3},  // AD
        pair<int, int>{1, 2},  // BC
        pair<int, int>{1, 4},  // BE
        pair<int, int>{2, 5},  // CF
        pair<int, int>{3, 4},  // DE
        pair<int, int>{4, 6},  // EG
        pair<int, int>{5, 7},  // FH
        pair<int, int>{6, 7}   // GH
    };

    vector<EdgeProperty> the_properties = {{5, 999001, "AB"}, {10, 999002, "AC"}, {1, 999003, "AD"}, {8, 999004, "BC"},
                                           {3, 999005, "BE"}, {5, 999006, "CF"},  {9, 999007, "DE"}, {5, 999008, "EG"},
                                           {2, 999009, "FH"}, {3, 999010, "GH"}};

    Graph mygraph(the_edges.begin(), the_edges.end(), the_properties.begin(), 8);
    assert(num_vertices(mygraph) == 8);
    assert(num_edges(mygraph) == 10);

    assert(mygraph[edge(0, 1, mygraph).first].name == "AB");

    // On va maintenant définir les propriétés des indice.
    // Pour récupérer un VertexDescriptor à partir de leur indice, je suppose (apparemment à raison) que les vertices
    // ont été insérés dans l'ordre de définition plus haut. (une alternative aurait été de récupérer les 8 vertex en 8
    // appels du genre "source(this_specific_edge, mygraph)", mais c'était moins pratique)

    // Cette lambda me permet de setter les propriétés d'un vertex à partir de son indice (indice utilisé dans le vector
    // the_edges plus haut) :
    auto set_vertex_property = [&mygraph](int vertex_index, string const& name, int altitude, long long tomtom_id) {
        auto vd = vertex(vertex_index, mygraph);
        mygraph[vd].name = name;
        mygraph[vd].altitude = altitude;
        mygraph[vd].tomtom_id = tomtom_id;
    };
    set_vertex_property(0, "A", 1111, 125001);
    set_vertex_property(1, "B", 2222, 125002);
    set_vertex_property(2, "C", 3334, 125003);
    set_vertex_property(3, "D", 4444, 125004);
    set_vertex_property(4, "E", 5555, 125005);
    set_vertex_property(5, "F", 6666, 125006);
    set_vertex_property(6, "G", 7777, 125007);
    set_vertex_property(7, "H", 8888, 125008);

    assert(mygraph[vertex(0, mygraph)].name == "A");

    EdgeIterator e_ite, e_end;
    // check que les edges et les vertex sont bien cohérents (la concaténation du name de source+target donne le name de
    // l'edge) :
    for (boost::tie(e_ite, e_end) = edges(mygraph); e_ite != e_end; ++e_ite) {
        auto v_source = source(*e_ite, mygraph);
        auto v_target = target(*e_ite, mygraph);
        auto edge_name = mygraph[*e_ite].name;
        assert(edge_name == mygraph[v_source].name + mygraph[v_target].name);
        cout << edge_name << endl;
    }

    cout << "ALL IS OK" << endl;
    return 0;
}

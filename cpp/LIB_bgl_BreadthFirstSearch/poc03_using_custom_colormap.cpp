#include <iostream>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace boost;
using namespace std;

// the rank property-map associates a vertex to the rank of its discovery
class BfsDiscoveryRankVisitor : public default_bfs_visitor
{
public:
    BfsDiscoveryRankVisitor(string& discovered_vertices_) : discovered_vertices{discovered_vertices_} {}
    template <typename VertexDescriptor, typename Graph>
    void discover_vertex(VertexDescriptor u, const Graph& mygraph) const
    {
        discovered_vertices.append(mygraph[u].name);
    }
    string& discovered_vertices;
};


char const* color_to_string(default_color_type c) {
    switch(c) {
        case default_color_type::gray_color:  return "gray";
        case default_color_type::white_color: return "white";
        case default_color_type::black_color: return "black";
        case default_color_type::green_color:  return "green";
        case default_color_type::red_color:  return "red";
    }
    return "unknown";
}


int main(int, char*[])
{
    cout << endl;
    cout << "Par rapport aux POCs précédentes, cette POC utilise une ColorMap custom pour le BFS" << endl;

    cout << "   - cette ColorMap custom permet de marquer des noeuds comme black (déjà explorés) avant le début de l'algo" << endl;
    cout << "   - on est responsable de créer la ColorMap (et initialiser tous les noeuds à white)" << endl;
    cout << "   - la fonction à appeler change : https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/breadth_first_visit.html" << endl;
    cout << "   - conséquence 1 = E n'est pas visité (et n'apparaît donc pas dans la string finale)" << endl;
    cout << "   - conséquence 2 = certains noeuds (e.g. G) sont visités plus tardivement, car par un autre chemin" << endl;
    cout << "   - note :si on est sûr que l'algo ne s'appliquera qu'à une sous-partie du graphe, on peut passer une colormap..." << endl;
    cout << "   - ... qui n'est définie que pour les vertex concernés (alors qu'avec breadth_first_search, l'algo commençait..." << endl;
    cout << "   - ... par initialiser tous les vertex à white -> la colormap devait nécessairement les connaître)" << endl;
    cout << endl;
    cout << "(cf. des notes générales sur les BFS et boost dans le script bash de build/run)" << endl;
    cout << endl;


    //========================================
    // STEP 1 = defining graph :
    struct VertexProperty {
        string name;
        double latitude;
        double longitude;
    };
    struct EdgeProperty {
        int weight;
        string roadname;
    };
    using Graph = adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty>;
    Graph mygraph;
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;

    //========================================
    // STEP 2 = creating vertices+edges :

    cout << endl;
    cout << "Le graphe construit va être celui-ci :   " << endl;
    cout << "           10                            " << endl;
    cout << "     ---------------                     " << endl;
    cout << "    /               \\                    " << endl;
    cout << "   /   5         8   \\                   " << endl;
    cout << "  A---------B---------C                  " << endl;
    cout << "  |         |         |                  " << endl;
    cout << "  |1        |3        |5                 " << endl;
    cout << "  |         |         |                  " << endl;
    cout << "  D---------E         F                  " << endl;
    cout << "       9    |         |                  " << endl;
    cout << "            |5        |2                 " << endl;
    cout << "            |         |                  " << endl;
    cout << "            G---------H                  " << endl;
    cout << "                 3                       " << endl;
    cout << endl;

    VertexDescriptor A = add_vertex({"A", 48.8472743, 2.3385463}, mygraph);
    VertexDescriptor B = add_vertex({"B", 48.8538388, 2.2667097}, mygraph);
    VertexDescriptor C = add_vertex({"C", 48.8822442, 2.3355303}, mygraph);
    VertexDescriptor D = add_vertex({"D", 48.8784585, 2.3682707}, mygraph);
    VertexDescriptor E = add_vertex({"E", 48.8783875, 2.2781327}, mygraph);
    VertexDescriptor F = add_vertex({"F", 48.8751455, 2.3496791}, mygraph);
    VertexDescriptor G = add_vertex({"G", 48.8695761, 2.3778204}, mygraph);
    VertexDescriptor H = add_vertex({"H", 48.8404808, 2.2935483}, mygraph);

    add_edge(A, B, {5, "Boulevard Saint-Michel"}, mygraph);
    add_edge(A, C, {10, "Avenue Mozart"}, mygraph);
    add_edge(A, D, {1, "Place Pigalle"}, mygraph);
    add_edge(B, C, {8, "Boulevard de la Vilette"}, mygraph);
    add_edge(B, E, {3, "Avenue de Neuilly"}, mygraph);
    add_edge(C, F, {5, "Rue de Paradis"}, mygraph);
    add_edge(D, E, {9, "Boulevard de Belleville"}, mygraph);
    add_edge(E, G, {5, "Rue Lecourbe"}, mygraph);
    add_edge(F, H, {2, "Avenue des Champs-Élysées"}, mygraph);
    add_edge(G, H, {3, "Rue de la Paix"}, mygraph);


    //========================================
    // STEP 3 = applying BFS :

    // custom ColorMap :
    vector<default_color_type> color_storage(num_vertices(mygraph), white_color);
    auto vertex_index_pmap = get(vertex_index, mygraph);
    auto color_pmap = make_iterator_property_map(color_storage.begin(), vertex_index_pmap);
    // all vertex are initially white, except E which is black :
    put(color_pmap, E, black_color);

    string discovered_vertices;
    BfsDiscoveryRankVisitor my_super_visitor(discovered_vertices);
    // breadth_first_visit is equivalent to breadth_first_search (except for the custom color_map)
    // see https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/breadth_first_visit.html
    breadth_first_visit(mygraph, vertex(B, mygraph), visitor(my_super_visitor).color_map(color_pmap));

    cout << "Here are the vertices, sorted by discovery-rank : " << discovered_vertices << endl;
    // expected order = BACDFHG
    cout << "Note that 'E' is never discovered (because it was marked as already discovered in the ColorMap)" << endl;
    cout << "Note that 'G' is discovered last (because without E, the path to G is the longest)" << endl;

    cout << "ALL IS OK" << endl;
    return 0;
}

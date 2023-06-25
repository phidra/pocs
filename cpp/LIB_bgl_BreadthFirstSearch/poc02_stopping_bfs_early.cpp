#include <iostream>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace boost;
using namespace std;

struct StopTheBFS : public runtime_error { StopTheBFS() : runtime_error{""} {} };

// the rank property-map associates a vertex to the rank of its discovery
class BfsDiscoveryRankVisitor : public default_bfs_visitor
{
public:
    BfsDiscoveryRankVisitor(string& discovered_vertices_) : discovered_vertices{discovered_vertices_} {}
    template <typename VertexDescriptor, typename Graph>
    void discover_vertex(VertexDescriptor u, const Graph& mygraph) const
    {
        discovered_vertices.append(mygraph[u].name);
        if (mygraph[u].name == "G") {
            throw StopTheBFS{};
        }
    }
    string& discovered_vertices;
};

int main(int, char*[])
{
    cout << endl;
    cout << "Par rapport à la POC précédente, cette POC stoppe le BFS dès que le noeud G a été trouvé" << endl;

    cout << "   - par conséquent, certains noeuds du graphe ne seront pas visités (au moins H)" << endl;
    cout << "   - suit les préconisations de l'item 1 de : https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/faq.html" << endl;
    cout << "   - (aka : le visiteur throw lorsqu'il visite le noeud d'arrêt, l'appelant catche)" << endl;
    cout << "   - par ailleurs, le stockage du discovery-rank est plus simple que dans la POC précédente" << endl;
    cout << "   - (le visitor construit une string décrivant l'ordre de découverte, plutôt qu'un vector)" << endl;
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

    string discovered_vertices;
    BfsDiscoveryRankVisitor my_super_visitor(discovered_vertices);
    try {
        breadth_first_search(mygraph, vertex(B, mygraph), visitor(my_super_visitor));
    }
    catch (StopTheBFS&) {
        // passing here is "normal", see https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/faq.html
    }

    cout << "Here are the vertices, sorted by discovery-rank : " << discovered_vertices << endl;
    // expected order = BACEDFG
    cout << "Note that 'H' is never discovered" << endl;
    cout << "(because we exited the BFS as soon as G was discovered)" << endl;
    cout << endl;

    cout << "ALL IS OK" << endl;
    return 0;
}

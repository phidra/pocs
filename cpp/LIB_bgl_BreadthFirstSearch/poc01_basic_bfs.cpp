#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <iostream>
#include <string>

using namespace boost;
using namespace std;

// the rank property-map associates a vertex to the rank of its discovery
template <typename RankPMap>
class BfsDiscoveryRankVisitor : public default_bfs_visitor {
    using T = typename property_traits<RankPMap>::value_type;

   public:
    BfsDiscoveryRankVisitor(RankPMap tmap, T& t) : m_rank_pmap(tmap), m_rank(t) {}
    template <typename VertexDescriptor, typename Graph>
    void discover_vertex(VertexDescriptor u, Graph const&) const {
        put(m_rank_pmap, u, m_rank++);
    }
    RankPMap m_rank_pmap;
    T& m_rank;
};

int main(int, char*[]) {
    cout << endl;
    cout << "Cette POC vise à faire un BFS/DFS sur un graphe donné, elle se contente de " << endl;
    cout << "   - construire le graphe, ses vertex/edges (et leur properties)" << endl;
    cout << "   - appliquer le BFS (en utilisant un visitor custom pour mémoriser le rank de visite des noeuds)"
         << endl;
    cout << "   - afficher les noeuds par rank de visite" << endl;
    cout << endl;
    cout << "Ce que cette POC démontre :" << endl;
    cout << "   - l'utiliation du BFS" << endl;
    cout << "   - la création d'un visitor custom" << endl;
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
    using Size = graph_traits<Graph>::vertices_size_type;
    Graph mygraph;
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;
    using VertexIterator = graph_traits<Graph>::vertex_iterator;
    auto nameof = [&mygraph](VertexDescriptor const& vd) { return mygraph[vd].name; };

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

    // vector storing the discovery rank of each vertex :
    vector<Size> discovery_ranks(num_vertices(mygraph));
    auto vertex_index_pmap = get(vertex_index, mygraph);
    auto discovery_rank_pmap = make_iterator_property_map(discovery_ranks.begin(), vertex_index_pmap);
    using DiscoveryRankPMap = decltype(discovery_rank_pmap);

    Size discovery_rank = 0;
    // the visitor will hold a reference to this discovery_rank
    // to understand why, see 2nd item in : https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/faq.html
    BfsDiscoveryRankVisitor<DiscoveryRankPMap> my_super_visitor(discovery_rank_pmap, discovery_rank);
    breadth_first_search(mygraph, vertex(B, mygraph), visitor(my_super_visitor));

    // from now on, discovery_rank_pmap associates each vertex to its discovery-rank.
    // copying the vertices in a vector that will be sorted :
    VertexIterator v, v_end;
    tie(v, v_end) = vertices(mygraph);
    vector<VertexDescriptor> vertices(v, v_end);
    for (auto vd : vertices) {
        cout << nameof(vd) << " was discovered in rank : " << get(discovery_rank_pmap, vd) << endl;
        ;
    }

    // sorting the vertices by their discovery ranks :
    sort(vertices.begin(),
         vertices.end(),
         [&discovery_rank_pmap](VertexDescriptor const& left, VertexDescriptor const& right) {
             auto left_discovery_rank = get(discovery_rank_pmap, left);
             auto right_discovery_rank = get(discovery_rank_pmap, right);
             return left_discovery_rank < right_discovery_rank;
         });

    string vertices_by_rank = accumulate(
        vertices.cbegin(), vertices.cend(), string{}, [&nameof](string const& acc, VertexDescriptor const& vd) {
            return acc + nameof(vd);
        });
    cout << "Here are the vertices, sorted by discovery-rank : " << vertices_by_rank << endl;
    // expected order = BACEDFGH

    cout << "ALL IS OK" << endl;
    return 0;
}

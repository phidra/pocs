#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace std;

int main(int,char*[])
{
    cout << endl;
    cout << "Cette POC reste basique mais apporte des réponses à quelques questions pratiques et concrètes :" << endl;
    cout << "   - attribution de propriétés aux vertices / edges (y compris custom)" << endl;
    cout << "   - accéder aux propriétés d'un vertex/edge" << endl;
    cout << "   - accéder au nombre d'edges / vertices" << endl;
    cout << "   - itérer sur les edges/vertices" << endl;
    cout << endl;


    // propriétés custom = tomtom_id :
    struct vertex_tomtom_id_t { using kind = vertex_property_tag; };
    struct edge_tomtom_id_t { using kind = edge_property_tag; };
    // chaque vertex a deux propriétés : son nom, et son id tomtom :
    using VertexProperty = property<vertex_name_t, std::string, property<vertex_tomtom_id_t, unsigned long> >;
    // chaque edge a deux propriétés : son coût, et son id tomtom :
    using EdgeProperty = property<edge_weight_t, int, property<edge_tomtom_id_t, unsigned long> >;
    // définition de mon graphe (avec propriété sur les vertices et les edges) :
    using Graph = adjacency_list<vecS, vecS, bidirectionalS, VertexProperty, EdgeProperty>;
    using EdgeDescriptor = graph_traits<Graph>::edge_descriptor;
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;
    using VertexIterator = graph_traits<Graph>::vertex_iterator;
    using EdgeIterator = graph_traits<Graph>::edge_iterator;

    // On va modéliser le graphe suivant :
    //        ---------
    //       /         \
    //      A-----B-----C
    //      |           |
    //      D-----E-----F
    Graph graph;
    // VERTICES :
    VertexDescriptor A = add_vertex(VertexProperty("A", 125000432), graph);
    VertexDescriptor B = add_vertex(VertexProperty("B", 125000134), graph);
    VertexDescriptor C = add_vertex(VertexProperty("C", 125000999), graph);
    VertexDescriptor D = add_vertex(VertexProperty("D", 125000888), graph);
    VertexDescriptor E = add_vertex(VertexProperty("E", 125000777), graph);
    VertexDescriptor F = add_vertex(VertexProperty("F", 125000666), graph);
    // EDGES :
    pair<EdgeDescriptor, bool> added = add_edge(A, B, EdgeProperty(76, 1250000110), graph);
    EdgeDescriptor edge_AB = added.first;
    add_edge(A, C, EdgeProperty(2534, 125000220), graph);
    add_edge(A, D, EdgeProperty(1121, 125000330), graph);
    add_edge(B, C, EdgeProperty(8500, 125000440), graph);
    add_edge(C, F, EdgeProperty(4100, 125000051), graph);
    add_edge(D, E, EdgeProperty(2222, 125000256), graph);
    add_edge(E, F, EdgeProperty(12, 125000019), graph);
    cout << "Nb edges = " << num_edges(graph) << endl;
    cout << "Nb vertices = " << num_vertices(graph) << endl;


    // Les EdgeDescriptor / VertexDescriptor sont les handle permettant de manipuler les edges/vertex.
    // On peut accéder (en temps constant, si je dis pas de bêtise) à une propriété d'un vertex donné :
    // Il faut passer le PropertyTag en premier paramètre (ici, un vertex_name_t vide) :
    string name = get(vertex_name_t{}, graph, A);
    unsigned long node_id = get(vertex_tomtom_id_t{}, graph, A);
    cout << "Single vertex properties :" << endl;
    cout << "   A name    = '" << name << "'" << endl;
    cout << "   A node_id = " << node_id << endl;

    int weight = get(edge_weight_t{}, graph, edge_AB);
    unsigned long edge_id = get(edge_tomtom_id_t{}, graph, edge_AB);
    cout << "Single edge properties :" << endl;
    cout << "   AB weight  = " << weight << endl;
    cout << "   AB edge_id = " << edge_id << endl;
    cout << endl;

    // Itération sur tous les vertices du graphe :
    cout << "Itération sur tous les vertices :" << endl;
    VertexIterator v, v_end;
    for (tie(v, v_end) = vertices(graph); v != v_end; ++v)
    {
        VertexDescriptor vd = *v;
        cout << "vertex " << get(vertex_name_t{}, graph, vd) << " has the tomtom id : " << get(vertex_tomtom_id_t{}, graph, vd) << endl;
    }
    cout << endl;

    cout << "Itération sur tous les edges :" << endl;
    auto edge_pair = edges(graph);
    EdgeIterator e_begin = edge_pair.first;
    EdgeIterator e_end = edge_pair.second;
    for (EdgeIterator e = e_begin; e != e_end; ++e)
    {
        EdgeDescriptor ed = *e;
        cout << "edge [" << get(vertex_name_t{}, graph, source(ed, graph)) << "->" << get(vertex_name_t{}, graph, target(ed, graph)) << "]";
        cout << " has weight " << get(edge_weight_t{}, graph, ed) << " and tomtom id " << get(edge_tomtom_id_t{}, graph, ed) << endl;
    }
    cout << endl;

    return 0;
}

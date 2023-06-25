#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>

using namespace boost;
using namespace std;

int main(int, char*[]) {
    cout << endl;
    cout << "Cette POC est surtout là pour illustrer l'ANCIENNE façon de manipuler les properties." << endl;
    cout << "(par opposition aux bundled-properties, qui sont la façon MODERNE de faire)" << endl;
    cout << endl;

    // définition (notamment, propriété custom vs. propriété "pré-définie")
    // définition (notamment le fait qu'il faille chaîner les propriétés
    // remplissage du graphe (= rien ne change)
    // accès aux propriétés des vertex / edges
    // accès à une pmap des propriétés

    // Définition d'une vertex-property en utilisant un tag pré-existant (= son nom) :
    // cf. https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/property.html
    using VertexProperty = property<vertex_name_t, std::string>;

    // NOTE : la liste des pre-existing tags est inhomogène dans la doc :
    //      https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/using_adjacency_list.html#sec:adjacency-list-properties
    //      https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/PropertyTag.html
    //      https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/adjacency_list.html
    // Si besoin, le code-source fait foi :
    //      https://github.com/boostorg/graph/blob/e4e12158e78157397254505f6cbae688e3e3174f/include/boost/graph/properties.hpp#L79
    //      https://www.boost.org/doc/libs/1_75_0/boost/graph/properties.hpp

    // Définition de deux edge-properties, l'une avec un tag pré-existant, l'autre avec un tag custom :
    struct edge_walk_allowed_t {
        using kind = edge_property_tag;
    };  // c'est comme ça qu'on définit un tag custom
    using EdgeProperty = property<edge_weight_t, int, property<edge_walk_allowed_t, bool> >;

    // La définition du graphe, et des properties des vertex+edges ne change pas :
    using Graph = adjacency_list<vecS, vecS, bidirectionalS, VertexProperty, EdgeProperty>;
    using EdgeDescriptor = graph_traits<Graph>::edge_descriptor;
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;

    // Le remplissage du graphe ne change pas. On va modéliser le graphe simple suivant :
    //
    //      A-----B
    //       \   /
    //        \ /
    //         C
    Graph mygraph;
    VertexDescriptor A = add_vertex(VertexProperty("A"), mygraph);
    VertexDescriptor B = add_vertex({"B"}, mygraph);
    VertexDescriptor C = add_vertex({"C"}, mygraph);
    add_edge(A, B, EdgeProperty(76, true), mygraph);
    add_edge(A, C, {2534, false}, mygraph);
    add_edge(B, C, {8500, true}, mygraph);
    EdgeDescriptor AB = edge(A, B, mygraph).first;
    assert(num_edges(mygraph) == 3);
    assert(num_vertices(mygraph) == 3);

    // On accède à une property d'un vertex (resp. edge) en passant une instance de PropertyTag à la fonction get :
    string name = get(vertex_name_t{}, mygraph, A);
    cout << "This vertex has the name : " << name << endl;

    // Ça fonctionne de façon identique pour les properties custom :
    bool edge_walk_allowed = get(edge_walk_allowed_t{}, mygraph, AB);
    cout << "Walk is " << (edge_walk_allowed ? "ALLOWED" : "FORBIDDEN") << " for pedestrians." << endl;

    // On peut évidemment utiliser put :
    put(edge_weight_t{}, mygraph, AB, 99999);

    // Pour récupérer une property-map, il faut également passer une instance du PropertyTag à la fonction get :
    // (c'est donc un peu différent de la façon de faire avec les bundeled-properties)
    auto weight_pmap = get(edge_weight_t{}, mygraph);

    // L'utilisation de la property-map à proprement parler n'est pas différente d'avec les bundled-properties :
    int weight = get(weight_pmap, AB);
    cout << "AB weight  = " << weight << endl;

    return 0;
}

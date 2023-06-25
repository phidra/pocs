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

int main(int,char*[])
{
    cout << endl;
    cout << "Cette POC me permet de m'approprier le code la POC précédente avec disjktra :" << endl;
    cout << "   - on définit un graphe (décrit en commentaire)" << endl;
    cout << "   - on lui applique dijkstra pour calculer les plus courts chemins" << endl;
    cout << "   - on reconstruit le plus court chemin d'un noeud à un autre" << endl;
    cout << endl;
    cout << "Ce que cette POC démontre :" << endl;
    cout << "   - la façon de définir des properties custom pour les vertex (resp. edges), de deux façons : ancienne (boost::property) ou nouvelle (bundled properties)" << endl;
    cout << "   - la façon d'associer PLUSIEURS properties aux vertex (resp. edges), y compris des properties custom (dont les tags n'existent pas dans BGL)" << endl;
    cout << "   - deux façons de créer un vertex (resp. edge) avec ses properties : en créant la property préalablement, ou à la volée via un initializer-list" << endl;
    cout << "   - la façon de lire les properties d'un vertex (resp. edge), avec l'ANCIENNE comme la NOUVELLE façon de définir les properties" << endl;
    cout << "   - la façon de récupérer un edge à partir de deux vertex" << endl;
    cout << endl;

    //========================================
    // STEP 1 = définition du graphe :

    // type-tag (je ne peux pas utiliser la même structure, car l'un doit avoir kind=edge_property_tag, et l'autre doit avoir vertex) :
    struct tomtom_vertex_id_t{ using kind = vertex_property_tag;};

    // chaque vertex a deux propriétés : son nom (string), et son id tomtom (long long) :
    // ANCIENNE FAÇON DE FAIRE = utilisation des boost::property :
    using VertexProperty = property<vertex_name_t, std::string, property<tomtom_vertex_id_t, long long> >;

    // chaque edge a deux propriétés : son coût (int) , et son id tomtom (long long) :
    // NOUVELLE FAÇON DE FAIRE = bundled properties (cf. https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/bundles.html) :
    struct EdgeProperty {
        int weight;
        long long tomtom_id;
    };

    // définition de mon graphe (avec propriété sur les vertices et les edges) :
    using Graph = adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty>;
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;
    using EdgeDescriptor = graph_traits<Graph>::edge_descriptor;
    using VertexIterator = graph_traits<Graph>::vertex_iterator;
    using EdgeIterator = graph_traits<Graph>::edge_iterator;

    // On va modéliser le graphe suivant :
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
    // On va chercher le plus court chemin de A à H (qui est ABEGH, de coût 5+3+5+3 = 16)
    // Sauf erreur, on devrait récupérer les plus courts chemins de A à tous les autres noeuds ?

    Graph mygraph;

    //========================================
    // STEP 2 = création des VERTICES :

    // FAÇON N°1 DE CONSTRUIRE UN VERTEX AVEC SES PROPERTIES = construction explicite des properties (pas très pratique) :
    VertexProperty propA;
    get_property_value(propA, vertex_name) = "A";
    get_property_value(propA, tomtom_vertex_id_t{}) = 125001;
    VertexDescriptor A = add_vertex(propA, mygraph);

    // FAÇON N°2 DE CONSTRUIRE UN VERTEX AVEC SES PROPERTIES = construction à la volée de la property :
    VertexDescriptor B = add_vertex({"B", 125002}, mygraph);
    VertexDescriptor C = add_vertex({"C", 125003}, mygraph);
    VertexDescriptor D = add_vertex({"D", 125004}, mygraph);
    VertexDescriptor E = add_vertex({"E", 125005}, mygraph);
    VertexDescriptor F = add_vertex({"F", 125006}, mygraph);
    VertexDescriptor G = add_vertex({"G", 125007}, mygraph);
    VertexDescriptor H = add_vertex({"H", 125008}, mygraph);

    // Itération sur tous les vertices du graphe + accès aux properties d'un vertex :
    cout << "Itération sur tous les vertices :" << endl;
    VertexIterator v, v_end;

    // FAÇON N°1 = récupérer d'abord la PropertyMap, pour l'utiliser plus tard :
    auto vertex_name_property_map = get(vertex_name_t{}, mygraph);

    for (tie(v, v_end) = vertices(mygraph); v != v_end; ++v)
    {
        VertexDescriptor vd = *v;
        // FAÇON N°1 (suite) = aller chercher dans la PropertyMap la property du vertex :
        auto name = get(vertex_name_property_map, vd);

        // FAÇON N°2 = récupérer directement la property via le graphe :
        auto tomtom_id = get(tomtom_vertex_id_t{}, mygraph, vd);
        cout << "vertex " << name << " has tomtom_vertex_id : " << tomtom_id << endl;
    }
    cout << endl;


    //========================================
    // STEP 3 = création des EDGES :

    // ici j'utilise directement la façon n°2 de construire un vertex avec ses properties :
    add_edge(A, B, {5, 999001}, mygraph);
    add_edge(A, C, {10, 999002}, mygraph);
    add_edge(A, D, {1, 999003}, mygraph);
    add_edge(B, C, {8, 999004}, mygraph);
    add_edge(B, E, {3, 999005}, mygraph);
    add_edge(C, F, {5, 999006}, mygraph);
    add_edge(D, E, {9, 999007}, mygraph);
    add_edge(E, G, {5, 999008}, mygraph);
    add_edge(F, H, {2, 999009}, mygraph);
    add_edge(G, H, {3, 999010}, mygraph);
    assert(num_edges(mygraph) == 10);

    // Itération sur tous les edges du graphe + accès aux properties d'un edge :
    cout << "Itération sur tous les edge :" << endl;
    EdgeIterator e, e_end;

    for (tie(e, e_end) = edges(mygraph); e != e_end; ++e)
    {
        EdgeDescriptor ed = *e;
        // FAÇON N°3 = accéder aux bundled-properties d'un edge se fait via l'operator[] sur le graph :
        cout << "edge with tomtom_id [" << mygraph[ed].tomtom_id << "] has weight = " << mygraph[ed].weight << endl;
    }
    cout << endl;


    //========================================
    // STEP 4 = application du dijkstra :
    std::vector<VertexDescriptor> predecessors(num_vertices(mygraph));
    auto predecessors_property_map = make_iterator_property_map(predecessors.begin(), get(vertex_index, mygraph));

    std::vector<int> tentative_distances(num_vertices(mygraph));
    auto tentative_distances_property_map = make_iterator_property_map(tentative_distances.begin(), get(vertex_index, mygraph));

    auto SOURCE = A;
    auto TARGET = H;

    dijkstra_shortest_paths(
        mygraph,
        SOURCE,
        weight_map(get(&EdgeProperty::weight, mygraph)).  // à la différence de la POC précédente, je dois préciser explicitement comment "trouver" le weight des edges
        predecessor_map(predecessors_property_map).
        distance_map(tentative_distances_property_map)
    );

    //========================================
    // STEP 5 = reconstruction du plus court chemin :

    // note : cette lambda me simplifie la vie pour récupérer le name d'un vertex :
    auto nameof = [&vertex_name_property_map](VertexDescriptor const & vd) { return get(vertex_name_property_map, vd); };

    // reconstruction (à rebours) du plus court chemin :
    vector<VertexDescriptor> shortest_path = {TARGET};
    VertexDescriptor current_predecessor = TARGET;
    auto next_predecessor = get(predecessors_property_map, current_predecessor);
    // lorsque le prédécesseur d'un vertex est lui-même, c'est qu'on est sur la source, ou sur un noeud injoignable depuis la source
    // cf. la doc de l'algo = https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/dijkstra_shortest_paths.html
    while (next_predecessor != current_predecessor)
    {
        shortest_path.push_back(next_predecessor);
        current_predecessor = next_predecessor;
        next_predecessor = get(predecessors_property_map, current_predecessor);
    }

    // on remet le chemin dans le bon sens :
    reverse(shortest_path.begin(), shortest_path.end());

    // affichage du shortest path :
    string shortest_path_str = accumulate(shortest_path.cbegin(), shortest_path.cend(), string{}, [&nameof](string const & a, VertexDescriptor b) { return a + nameof(b); });
    const string EXPECTED_SHORTEST_PATH = "ABEGH";
    cout << "Shortest-path from " << nameof(SOURCE) << " to " << nameof(TARGET) << " is :" << endl;
    cout << shortest_path_str << endl;
    cout << "Expected shortest-path :" << endl;
    cout << EXPECTED_SHORTEST_PATH << endl;
    cout << endl;

    // affichage du poids des arcs :
    cout << "Le plus court chemin de " << nameof(SOURCE) << " vers " << nameof(TARGET) << " a pour poids total : " << get(tentative_distances_property_map, TARGET) << endl;
    auto source_iterator = shortest_path.begin();
    auto target_iterator = ++shortest_path.begin();
    while (target_iterator != shortest_path.end()) {
        auto edge_pair = edge(*source_iterator, *target_iterator, mygraph);
        auto edge = edge_pair.first;
        auto weight = mygraph[edge].weight;

        cout << nameof(*source_iterator) << "->" << nameof(*target_iterator) << " = " << weight << endl;

        ++source_iterator;
        ++target_iterator;
    }

    cout << endl;
    cout << "Expected edges weight on shortest-path : " << endl;
    cout << "A->B = 5" << endl;
    cout << "B->E = 3" << endl;
    cout << "E->G = 5" << endl;
    cout << "G->H = 3" << endl;
    cout << endl;

   // QUESTION 1 = si j'ai une paire de vertex (source+target), comment puis-je savoir si l'edge source->target existe, et en récupérer un descriptor ?
   // a priori, on peut le faire avec adjacency_matrix, mais pas avec adjacency_list
   // EDIT : ah ben si, ça marche aussi avec adjacency_list, apparemment :
    auto my_super_edge_pair = edge(A, B, mygraph);
    bool does_the_edge_exist = my_super_edge_pair.second;
    assert(does_the_edge_exist == true);
    auto my_super_edge = my_super_edge_pair.first;
    cout << "weight = " << mygraph[my_super_edge].weight << endl;
    cout << "source = " << nameof(source(my_super_edge, mygraph)) << endl;
    cout << "target = " << nameof(target(my_super_edge, mygraph)) << endl;
    cout << endl;

   // QUESTION 2 = du coup, que se passe-t-il si on essaye d'accéder à un edge qui n'existe pas ?
   // réponse = le booléen indique que l'edge n'exiset pas
    auto other_edge_pair = edge(A, H, mygraph);
    bool other_edge_exist = other_edge_pair.second;
    assert(other_edge_exist == false);

    return 0;
}

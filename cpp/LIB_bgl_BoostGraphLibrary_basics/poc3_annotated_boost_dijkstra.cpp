//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee,
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

int main(int, char *[])
{
    std::cout << std::endl;
    std::cout << "Cette POC n'est qu'une version annotée de l'exemple donné avec boost — mais c'est déjà pas mal !" << std::endl;
    std::cout << "    https://www.boost.org/doc/libs/1_74_0/libs/graph/example/dijkstra-example.cpp" << std::endl;
    std::cout << std::endl;

    // ========================================
    // STEP 1 = définition du type template du graphe :
    typedef adjacency_list < listS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;
    // les vertex n'ont pas d'internal-property
    // les edges ont l'internal-property "edge_weight_t" stocké dans des ints
    // comme rien n'est précisé explicitement, la directness du graphe a la valeur par défaut pour adjacency_list = directed
    //
    typedef graph_traits < Graph >::vertex_descriptor vertex_descriptor;


    // ========================================
    // STEP 2 = toute cette STEP ne sert qu'à appeler le constructeur du graphe pour l'initialiser avec les bons vertex/edges .
    // je suppose (à confirmer) qu'on pourrait remplacer tout ça par des add_edge / add_vertex explicites
    typedef std::pair<int, int> Edge;

    const int num_nodes = 5;
    // ceci est une simple façon de déclarer A=0, B=1, etc.
    // enum nodes { A, B, C, D, E };
    // d'ailleurs, je préfère la version explicite :
    int A = 0;
    int B = 1;
    int C = 2;
    int D = 3;
    int E = 4;

    // ceci est une simple façon de définir un tableau où l'index d'un node pointe sur son nom :
    char name[] = "ABCDE";

    Edge edge_array[] = {
        Edge(A, C), Edge(B, B), Edge(B, D),
        Edge(B, E), Edge(C, B), Edge(C, D),
        Edge(D, E), Edge(E, A), Edge(E, B)
    };
    int weights[] = { 1, 2, 1, 2, 7, 3, 1, 1, 1 };
    int num_arcs = sizeof(edge_array) / sizeof(Edge);

    // appel au 6ième constructor de la section "Member functions" de cette page : https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/adjacency_list.html
    Graph graph(edge_array, edge_array + num_arcs, weights, num_nodes);


    // ========================================
    // STEP 3 = accès facile au weight d'un edge donné :
    // ceci est simplement la récupération de la property_map des weights, permettant, à partir d'un edge donné, de récupérer son weight :
    property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, graph);


    // ========================================
    // STEP 4 = initialisation des property_maps nécessaires à l'algo :
    // cf. ma POC sur les property_map pour plus de détail, mais en gros :
    //  - la property-map utilise un vector comme storage
    //  - pour faire le lien entre un vertex_descriptor et ce storage-vector (OffsetMap), on utilise le vertex-index
    //  - (il associe un vertex-descriptor à un vertex, ce qui est bien ce qu'on veut faire)
    std::vector<vertex_descriptor> parents(num_vertices(graph));
    auto parents_property_map = boost::make_iterator_property_map(parents.begin(), get(boost::vertex_index, graph));

    std::vector<int> distances(num_vertices(graph));
    auto distances_property_map = boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, graph));

    // ========================================
    // STEP 5 = lancement de l'algo à proprement parler :
    vertex_descriptor SOURCE = vertex(A, graph);
    dijkstra_shortest_paths(
        graph,
        SOURCE,
        // ci-dessous, on utilise les named-parameters : https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/bgl_named_params.html
        // la liste des named-parameters acceptés par l'algo est dans la doc de l'algo : https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/dijkstra_shortest_paths.html
        predecessor_map(parents_property_map).  // ceci est une ReadWritePropertyMap stockant pour chaque vertex V, son prédécesseur sur le plus court chemin reliant SOURCE à V
        distance_map(distances_property_map)  // ceci est une ReadWritePropertyMap stockant pour chaque vertex V sa tentative-distance
    );

    // ========================================
    // STEP 6 = on affiche les distances et les parents (permettant de reconstituer si besoin les plus courts chemins) :
    // rien de sorcier ici, hormis le fait que c'est parce qu'on a utilisé boost::vertex_index comme OffsetMap qu'on peut utiliser la syntaxe bien pratique :
    //      distances[*vertex_descriptor]   ou    parents[*vertex_descriptor]
    std::cout << "distances and parents:" << std::endl;
    graph_traits < Graph >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(graph); vi != vend; ++vi) {
        std::cout << "distance(" << name[*vi] << ") = " << distances[*vi] << ", ";
        std::cout << "parent(" << name[*vi] << ") = " << name[parents[*vi]] << std::
            endl;
    }
    std::cout << std::endl;


    // ========================================
    // STEP 7 = on génère le dot du graphe :
    std::ofstream dot_file("NOGIT_dijkstra_graph.dot");
    dot_file << "digraph D {\n"
        << "  rankdir=LR\n"
        << "  size=\"4,3\"\n"
        << "  ratio=\"fill\"\n"
        << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

    // rien de sorcier ici non plus, les points notables sont :
    //      - on récupère les vertex source/target d'un edge (ou plutôt, leur descriptor)
    //      - on on récupère le poids d'un edge, en utilisant l'interface property_map (de la weightmap) alors même que c'était une internal-property :)
    // le seul point qui m'interpelle un peu dans ce code, c'est qu'on assimile allégrement le vertex_descriptor à son index dans l'enum
    // (alors que je m'attendrais à ce que ce lien soit au mieux fait la fonction "vertex" , au pire ne puisse pas être fait directement)
    graph_traits < Graph >::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) {
        graph_traits < Graph >::edge_descriptor e = *ei;
        graph_traits < Graph >::vertex_descriptor
            u = source(e, graph), v = target(e, graph);  // ici, u et v sont des vertex_descriptor

        // name est un array de char, dont on récupère les char par un index
        // le fait qu'on puisse faire  name[u]  veut dire qu'on utilise le vertex_descriptor comme un int
        // JE TROUVE CECI TRÈS SURPRENANT !
        // obviously ça marche, mais je m'attendrais à ce qu'on doive utiliser la fonction "vertex" décrite ici :
        //    https://www.boost.org/doc/libs/1_74_0/libs/graph/doc/adjacency_list.html
        //    vertex_descriptor
        //    vertex(vertices_size_type n, const adjacency_list& g)
        //    Returns the nth vertex in the graph's vertex list.
        dot_file << name[u] << " -> " << name[v]
            << "[label=\"" << get(weightmap, e) << "\"";
        if (parents[v] == u)
            dot_file << ", color=\"black\"";
        else
            dot_file << ", color=\"grey\"";
        dot_file << "]";
    }
    dot_file << "}";
    return EXIT_SUCCESS;
}


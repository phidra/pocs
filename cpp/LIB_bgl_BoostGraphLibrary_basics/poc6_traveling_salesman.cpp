#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <numeric>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/metric_tsp_approx.hpp>
#include <cassert>

using namespace boost;
using namespace std;

int main(int,char*[])
{
    cout << endl;
    cout << "Cette POC montre une utiliation concrète de la BGL = résolution du traveling salesman problem" << endl;
    cout << "   - on construit un graphe avec des villes et leurs distances" << endl;
    cout << "   - on applique l'algo 'metric_tsp_approx_tour_from_vertex'" << endl;
    cout << "   - on affiche les résultats" << endl;
    cout << endl;
    cout << "Ce que cette POC démontre :" << endl;
    cout << "   - l'utilisation d'adjacency_matrix (jusqu'ici, j'utilisais plutôt adjacency_list)" << endl;
    cout << "   - comment appliquer l'algo résolvant le TSP" << endl;
    cout << "   - l'utilisation de back_inserter pour créer un OutputIterator requis par l'algo (la doc n'était pas très explicite...)" << endl;
    cout << endl;

    // Pour les villes et leurs distances, je garde les 8 premières villes de ce site :
    //      http://chri2neux.free.fr/img/z_distances_france.pdf
    // (ça laisse tout de même 2520 chemins possibles, cf. https://fr.wikipedia.org/wiki/Probl%C3%A8me_du_voyageur_de_commerce)

    // Amélioration possible = ajouter les coordonnées des villes + produire un geojson du trajet en sortie ?

    // NOTE : je me suis appuyé sur :
    //  - https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/metric_tsp_approx.html
    //  - https://www.boost.org/doc/libs/1_66_0/libs/graph/test/metric_tsp_approx.cpp

    //========================================
    // STEP 1 = définition du graphe :
    //
    struct VertexProperty { string city_name; };
    struct EdgeProperty {
        EdgeProperty() : weight(0) {};
        EdgeProperty(int x) : weight(x) {};
        int weight;
    };
    using Graph = adjacency_matrix<undirectedS, VertexProperty, EdgeProperty>;
    using VertexDescriptor = graph_traits<Graph>::vertex_descriptor;


    //========================================
    // STEP 2 = préparation des edges et de leur poids :

    vector<string> cities = {
        "Amiens",
        "Angers",
        "Biarritz",
        "Bordeaux",
        "Brest",
        "Calais",
        "Cherbourg",
        "Clermond-Ferrand"
    };

    vector<vector<int>> distance_matrix = {
        {0,   369, 862, 679, 619, 159, 366, 524},  // from Amiens to ...
        {399, 0,   518, 335, 371, 494, 290, 402},  // from Angers to ...
        {862, 518, 0,   183, 817, 997, 808, 555},  // from Biarritz to ...
        {679, 335, 183, 0,   634, 814, 625, 369},
        {619, 371, 817, 634, 0,   714, 402, 752},
        {159, 494, 997, 814, 714, 0,   461, 672},
        {366, 290, 808, 625, 402, 461, 0,   647},
        {524, 402, 555, 369, 752, 672, 647, 0}
    };

    int number_of_cities = cities.size();
    int number_of_edges = (
            (number_of_cities * number_of_cities) // taille de la matrice...
            - number_of_cities                    // ... mais on ne compte pas la diagonale ...
            ) / 2                                 // ... et comme les edges sont bidirectionnels, on ne garde qu'un edge sur deux
    ;

    vector<pair<int, int>> the_edges;
    vector<EdgeProperty> the_distances;

    int source_city = 0;
    for(auto source_city_distances: distance_matrix) {
        assert(source_city_distances.size() == number_of_cities);

       int target_city = 0;
       for (auto weight: source_city_distances) {
           if (source_city != target_city) {
               assert(weight > 0);
               the_distances.emplace_back(weight);
               the_edges.emplace_back(source_city, target_city);
           }
           ++target_city;
       }
        ++source_city;
    }

    // à ce stade, les edges et leur poids sont correctement définis (mais sont dupliqués, ce qui n'est pas très grave) :
    assert(the_edges.size() == 2 * number_of_edges);
    assert(the_distances.size() == 2 * number_of_edges);


    //========================================
    // STEP 3 = création du graphe à partir de ses edges :
    Graph mygraph(the_edges.cbegin(), the_edges.cend(), the_distances.cbegin(), number_of_cities);
    assert(num_vertices(mygraph) == number_of_cities);
    assert(num_edges(mygraph) == number_of_edges);
    cout << "Il y a " << number_of_cities << " villes" << endl;
    cout << "Il y a " << number_of_edges << " edges" << endl;
    cout << endl;


    //========================================
    // STEP 4 = on ajoute les properties des vertex, à savoir le nom des villes :
    for (int city_index = 0; city_index != cities.size(); ++city_index) {
        // ceci suppose que les vertex ont été créé dans le même ordre que celui par lequel ils sont référencés dans the_edges :
        auto city_vertex = vertex(city_index, mygraph);
        mygraph[city_vertex].city_name = cities[city_index];
    }

    //========================================
    // STEP 5 = vérification manuelle de quelques edges et names, histoire d'être sûr :

    // note : cette lambda me simplifie la vie pour récupérer le name d'un vertex :
    auto nameof = [&mygraph](VertexDescriptor const & vd) { return mygraph[vd].city_name; };

    auto amiens_biarritz = edge(0, 2, mygraph).first;
    assert(nameof(source(amiens_biarritz, mygraph)) == "Amiens");
    assert(nameof(target(amiens_biarritz, mygraph)) == "Biarritz");
    assert(mygraph[amiens_biarritz].weight == 862);

    auto clermont_cherbourg = edge(7, 6, mygraph).first;
    assert(nameof(source(clermont_cherbourg, mygraph)) == "Clermond-Ferrand");
    assert(nameof(target(clermont_cherbourg, mygraph)) == "Cherbourg");
    assert(mygraph[clermont_cherbourg].weight == 647);


    //========================================
    // STEP 6 = application de l'heuristique résolvant le TSP :
    auto SOURCE = vertex(3, mygraph);
    auto weight_map = get(&EdgeProperty::weight, mygraph);
    vector<VertexDescriptor> results;
    auto result_iterator = back_inserter(results);

    // NOTE : au sujet de back_inserter :
    // https://en.cppreference.com/w/cpp/iterator/back_inserter
    //      back_inserter is a convenient function template that constructs a std::back_insert_iterator for the container c
    // https://en.cppreference.com/w/cpp/iterator/back_insert_iterator
    //      The container's push_back() member function is called whenever the iterator (whether dereferenced or not) is assigned to.

    metric_tsp_approx_tour_from_vertex(
        mygraph,
        SOURCE,
        weight_map,
        result_iterator
    );


    //========================================
    // STEP 7 = affichage du tour calculé :
    cout << "On commence le tour à la ville : " << nameof(SOURCE) << endl;
    cout << "Et le tour passe par les villes suivantes :" << endl;
    for (auto city_vertex: results) {
        cout << "\t" << nameof(city_vertex) << endl;
    }
    cout << endl;
}

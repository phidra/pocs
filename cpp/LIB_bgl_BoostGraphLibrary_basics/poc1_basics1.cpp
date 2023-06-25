#include <iostream>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace std;

int main(int,char*[])
{

    cout << endl;
    cout << "Cette POC est une simple prise de contact avec la librairie." << endl;
    cout << "Elle soulève beaucoup de questions..." << endl;
    cout << endl;

    // définition de mon graphe (sans propriété ni sur les vertices, ni sur les edges) :
    using Graph = adjacency_list<vecS, vecS, bidirectionalS>;
    using EdgeDescriptor = graph_traits<Graph>::edge_descriptor;
    using AdjacencyIterator = graph_traits<Graph>::adjacency_iterator;

    // On va modéliser le graphe suivant :
    //        ---------
    //       /         \
    //      0-----1-----2
    //      |      \ _  |
    //      |         \ |
    //      3-----4-----5
    Graph graph;

    // pair<EdgeDescriptor, bool> ret1 = add_edge("vertice1", 2, graph);  // add_edge ne fonctionne pas avec des strings
    pair<EdgeDescriptor, bool> returned = add_edge(0, 1, graph);
    cout << "First edge created ? " << boolalpha << returned.second << endl;
    cout << "First edge = " << returned.first << endl;
    // À partir d'un edge, on peut récupérer sa source et sa target :
    cout << "First edge source = " << source(returned.first, graph) << endl;
    cout << "First edge target = " << target(returned.first, graph) << endl;
    cout << endl;
    add_edge(0, 2, graph);
    add_edge(0, 3, graph);
    add_edge(1, 2, graph);
    add_edge(1, 5, graph);
    add_edge(2, 5, graph);
    add_edge(3, 4, graph);
    add_edge(4, 5, graph);

    // PAS CLAIR = comment ai-je pu appeler "add_edge" sans avoir défini ce que contenait un edge ?
    //             j'ai pu vérifier que add_edge ne marchait pas avec une string -> peut-être que l'int
    //             est le type de vertice par défaut ?
    //             EDIT : on dirait qu'il s'agit de l'index du vertex dans le vector ?

    // QUESTION = à partir du graphe, comment récupérer le nombre d'edges ? de vertices ?
    cout << "Nb edges = " << num_edges(graph) << endl;
    cout << "Nb vertices = " << num_vertices(graph) << endl;

    // Itération sur les vertices adjacents au vertice 0 :
    cout << "Itération sur les vertices adjacents au vercice 0 :" << endl;
    AdjacencyIterator ai;
    AdjacencyIterator ai_end;
    for (tie(ai, ai_end) = adjacent_vertices(0, graph); ai != ai_end; ++ai)
    {
        cout << *ai << endl;
    }
    cout << "FIN" << endl;

    return 0;
}

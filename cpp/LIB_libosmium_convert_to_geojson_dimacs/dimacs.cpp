#include "dimacs.h"

#include <fstream>
#include <iomanip>
#include <iostream>

void create_dimacs(std::string output_dir,
                   std::map<int, osmium::Location> const& nodes,
                   std::vector<Edge> const& edges) {
    // objectif = générer le fichier de nodes suivant :
    // p aux sp co 12703
    // v 1 7.4174923 43.7296303
    // v 2 7.4174638 43.7296727
    // [...] 12703 lignes 'v' au total, chacune représentant un node
    std::ofstream dimacs_nodes(output_dir + "nodes.co");
    auto const nb_nodes = nodes.size();
    dimacs_nodes << "p aux sp co " << nb_nodes << std::endl;
    dimacs_nodes << std::setprecision(8);
    for (auto nodepair : nodes) {
        auto const& node_index = nodepair.first;
        auto const& node_location = nodepair.second;
        dimacs_nodes << "v " << node_index << " " << node_location.lon() << " " << node_location.lat() << "\n";
    }

    // objectif = générer le fichier d'edges suivant :
    // p sp 12703 10492
    // a 1 2 5
    // a 2 3 5
    // [...] 10492 lignes 'a' au total, chacune représentant un edge
    std::ofstream dimacs_edges(output_dir + "edges.gr");
    dimacs_edges << "p sp " << nb_nodes << " " << edges.size() << std::endl;
    dimacs_edges << std::fixed << std::setprecision(1);  // 1 digit after the comma
    for (auto edge : edges) {
        dimacs_edges << "a " << edge.node_from << " " << edge.node_to << " " << edge.length_m << "\n";
    }
}

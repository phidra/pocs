#include "graph.h"

#include <osmium/geom/haversine.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_mmap_array.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>

#include "graph/osmparsing.h"

using namespace std;

float geometry_length_in_meters(Polyline const& geometry) {
    // precondition = polyline has at least 2 points
    Polyline::const_iterator first = geometry.begin();
    Polyline::const_iterator second = first + 1;

    float total_length = 0;
    while (second != geometry.end()) {
        total_length += osmium::geom::haversine::distance(*first, *second);
        ++first;
        ++second;
    }
    return total_length;
}

static void add_edge(vector<Edge>& edges,
                     osmium::object_id_type node_from,
                     osmium::object_id_type node_to,
                     Polyline&& geometry) {
    float length_m = geometry_length_in_meters(geometry);
    float weight = length_m;
    edges.emplace_back(node_from, node_to, std::move(geometry), length_m, weight);
}

// precondition = chaque way a au moins 2 nodes
std::vector<Edge> build_graph(std::map<WayId, std::vector<LocatedNode> > const& way_to_nodes,
                              std::map<NodeOsmId, int> const& number_of_node_usage) {
    vector<Edge> edges;

    // L'objectif de ce traitement est de splitter les ways en plusieurs edges si nécessaire.
    // Basiquement, dans la donnée OSM, on peut avoir la situation suivante :
    //
    // A----------(x)-----------B
    //             |
    //             |
    //             C
    // Ici, la donnée OSM n'a que DEUX ways : [AB] et [xC]
    // Or, dans le graphe, pour permettre le routing (par exemple pour permettre de faire A->x->C), il faut TROIS edges.
    // On cherche donc à splitter la way AB en deux edges Ax et xB, et à créer un edge xC.

    // itérer sur toutes les ways
    for (auto ite : way_to_nodes) {
        auto wayid = ite.first;
        auto nodes = ite.second;

        // pour chaque way, itérer sur les nodes
        auto first_node = nodes.begin();
        auto last_node = (nodes.end() - 1);
        while (first_node != last_node) {
            auto second_node = (first_node + 1);
            Polyline geometry{first_node->second};

            // note : pour ne pas laisser de côté les impasses, il faut obligatoirement ajouter le premier et dernier
            // node, même s'ils ont un compteur à 1

            // skipping all nodes that only belong to this way :
            while (second_node != nodes.end() && number_of_node_usage.at(second_node->first) < 2) {
                geometry.push_back(second_node->second);
                ++second_node;
            }

            // à ce stade, second_node pointe vers le premier node (après first_node) qui a un compteur >= 2
            // (ou bien vers end() s'il n'y en avait pas, ce qui veut dire que la way était une impasse se finissant sur
            // le dernier noeud)

            // si second_node est à end, c'est que le dernier node de la way n'avait pas un compteur >= 2
            // Dit autrement : la way était une impasse, se terminant sur second_node.
            // Dans ce cas, on ajoute l'edge, et on a fini pour cette way :
            if (second_node == nodes.end()) {
                add_edge(edges, first_node->first, (second_node - 1)->first, std::move(geometry));
                break;
            }

            // cas général : on ajoute l'edge, et on continue d'iéterer sur la way :
            geometry.push_back(second_node->second);
            add_edge(edges, first_node->first, second_node->first, std::move(geometry));
            first_node = second_node;

            // NOTE : quoi qu'il arrive, on aura au moins un edge ajouté contenant le premier node, et un (autre,
            // peut-être) edge contenant le dernier node.
        }
    }

    return edges;
}

vector<Edge> osm_to_graph(string osmfile, BgPolygon polygon) {
    // osmium warmup :
    auto interesting_types = osmium::osm_entity_bits::node | osmium::osm_entity_bits::way;
    using Index = osmium::index::map::SparseMmapArray<osmium::unsigned_object_id_type, osmium::Location>;
    Index index;
    osmium::handler::NodeLocationsForWays<Index> location_handler{index};
    FillingHandler handler{polygon};
    osmium::io::Reader reader{osmfile, interesting_types};

    // parse osmfile + fill-in data structures :
    osmium::apply(reader, location_handler, handler);
    reader.close();

    // build graph edges :
    auto edges = build_graph(handler.way_to_nodes, handler.node_use_counter);
    return edges;
}

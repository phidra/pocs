#include <iostream>
#include <osmium/geom/haversine.hpp>
#include <osmium/handler.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_mmap_array.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>

#include "dimacs.h"
#include "edge.h"
#include "geojson.h"

// libosmium Handler that fills way+nodes structures
struct MyHandler : public osmium::handler::Handler {
    std::map<osmium::unsigned_object_id_type, int> way_osmid_to_index;
    std::map<int, osmium::unsigned_object_id_type> way_index_to_osmid;
    std::map<int, double> way_index_to_length;
    std::map<int, std::vector<osmium::Location> > way_index_to_geometry;
    int current_way_index = 0;

    std::vector<Edge> edges;

    std::map<osmium::unsigned_object_id_type, int> node_osmid_to_index;
    std::map<int, osmium::unsigned_object_id_type> node_index_to_osmid;
    std::map<int, osmium::Location> node_index_to_location;
    int current_node_index = 0;

    void way(osmium::Way const& way) noexcept {
        auto tag_highway = way.tags()["highway"];
        // FIXME : we would probably like to filter out non-pedestrian ways.
        // For now, we keep all ways with tag highway set :
        if (tag_highway == nullptr)
            return;

        double length_m = osmium::geom::haversine::distance(way.nodes());

        // update way structures :
        way_index_to_osmid.emplace(current_way_index, way.id());
        way_index_to_length.emplace(current_way_index, length_m);

        bool at_least_one_valid_node = false;
        int node_from = current_node_index;

        way_index_to_geometry.emplace(current_way_index, std::vector<osmium::Location>());
        for (auto const& node : way.nodes()) {
            auto const& loc = node.location();
            if (loc.valid()) {
                // update node structures :
                node_index_to_osmid.emplace(current_node_index, node.ref());
                node_index_to_location.emplace(current_node_index, loc);
                auto returned = node_osmid_to_index.emplace(node.ref(), current_node_index);
                auto new_node_was_inserted = returned.second;
                if (new_node_was_inserted) {
                    ++current_node_index;
                }

                way_index_to_geometry[current_way_index].push_back(node.location());

                at_least_one_valid_node = true;
            } else {
                throw std::string("NOT IMPLEMENTED: can't handle node with invalid location : ") +
                    std::to_string(current_node_index);
            }
        }
        int node_to = current_node_index - 1;
        edges.emplace_back(node_from, node_to, length_m);

        if (!at_least_one_valid_node) {
            throw std::string("NOT IMPLEMENTED: can't handle when there is no valid node, for way : ") +
                std::to_string(current_way_index);
        }

        auto returned = way_osmid_to_index.emplace(way.id(), current_way_index);
        auto new_way_was_inserted = returned.second;
        if (new_way_was_inserted)
            ++current_way_index;
    }
};

void debug_display(MyHandler const& handler) {
    // first ways :
    int counter = 0;
    for (auto pair : handler.way_index_to_osmid) {
        if (counter++ < 5)
            std::cout << "[way " << pair.first << "] = https://www.openstreetmap.org/way/" << pair.second << std::endl;
    }

    // first nodes :
    counter = 0;
    for (auto pair : handler.node_index_to_osmid) {
        if (counter++ < 5)
            std::cout << "[node " << pair.first << "] = https://www.openstreetmap.org/node/" << pair.second
                      << std::endl;
    }
}

int main(int argc, char* argv[]) {
    auto time_before = std::chrono::steady_clock::now();

    if (argc != 3) {
        std::cerr << "From a given OSM file, will create dimacs + geojson in output-dir.\n";
        std::cerr << "Usage: \n" << argv[0] << " OSMFILE  OUTPUT-DIR\n";
        std::cerr << "\t" << argv[0] << " OSMFILE  OUTPUT-DIR\n";
        std::cerr << "For instance : \n";
        std::cerr << "\t" << argv[0] << " NOGIT_extract.osm.pbf  /tmp/published\n";
        std::exit(1);
    }

    try {
        auto osmfile = argv[1];
        std::string output_dir(argv[2]);
        if (output_dir.back() != '/') {
            output_dir.push_back('/');
        }

        auto interesting_types = osmium::osm_entity_bits::node | osmium::osm_entity_bits::way;
        osmium::io::Reader reader{osmfile, interesting_types};

        // location handler -> sette les locations des nodes des ways
        using Index = osmium::index::map::SparseMmapArray<osmium::unsigned_object_id_type, osmium::Location>;
        using LocationHandler = osmium::handler::NodeLocationsForWays<Index>;
        Index index;
        LocationHandler location_handler{index};

        MyHandler handler;
        osmium::apply(reader, location_handler, handler);
        reader.close();

        create_geojson(output_dir, handler.way_index_to_length, handler.way_index_to_geometry);
        create_dimacs(output_dir, handler.node_index_to_location, handler.edges);
        debug_display(handler);

    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
        std::exit(1);
    }
    return 0;
}

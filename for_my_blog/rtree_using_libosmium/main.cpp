#include <boost/geometry.hpp>
#include <iostream>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_mmap_array.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

using Point = bg::model::point<double, 2, bg::cs::spherical_equatorial<bg::degree> >;
using NodePoint = std::pair<Point, osmium::object_id_type>;
using RTree = bgi::rtree<NodePoint, bgi::linear<8> >;

struct RTreeFillingHandler : public osmium::handler::Handler {
    explicit RTreeFillingHandler(RTree& rtree) : m_rtree{rtree} {}
    void way(osmium::Way const& w) {
        if (w.tags()["highway"] != nullptr)  // we're only interested in highways
        {
            if (w.nodes().size() >= 2) {
                auto front = w.nodes().front();
                m_rtree.insert({{front.location().lon(), front.location().lat()}, front.ref()});
                auto back = w.nodes().back();
                // this might insert duplicates. For our usage, no big deal :
                m_rtree.insert({{back.location().lon(), back.location().lat()}, back.ref()});
            }
        }
    }
    RTree& m_rtree;
};

RTree build_rtree_from_osm_data(char const* const osmfile) {
    RTree rtree;
    try {
        auto relevant_types = osmium::osm_entity_bits::node | osmium::osm_entity_bits::way;
        osmium::io::Reader reader{osmfile, relevant_types};

        // location handler, needed to use nodes' locations :
        using Index = osmium::index::map::SparseMmapArray<osmium::unsigned_object_id_type, osmium::Location>;
        using LocationHandler = osmium::handler::NodeLocationsForWays<Index>;
        Index index;
        LocationHandler location_handler{index};

        RTreeFillingHandler rtree_handler{rtree};
        osmium::apply(reader, location_handler, rtree_handler);
        reader.close();

    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
        std::exit(2);
    }
    return rtree;
}

std::tuple<Point, osmium::object_id_type, float> find_nearest(RTree const& rtree, Point const& requested_point) {
    std::vector<NodePoint> result;
    auto nb_found = rtree.query(bgi::nearest(requested_point, 1), std::back_inserter(result));
    if (nb_found == 0) {
        std::cerr << "unable to find nearest value in rtree (currently containing " << rtree.size() << " segments)"
                  << '\n';
        std::exit(3);
    }

    auto found = result.front();
    auto distance = bg::distance(found.first, requested_point, bg::strategy::distance::haversine<double>(6'371'000));
    return std::make_tuple(found.first, found.second, static_cast<float>(distance));
}

void display(std::tuple<Point, osmium::object_id_type, float>& nearest, float MAX_ALLOWED_DISTANCE_IN_M) {
    Point const& found_node = std::get<0>(nearest);
    osmium::object_id_type node_id = std::get<1>(nearest);
    float distance = std::get<2>(nearest);

    std::cout << "Nearest point is " << std::setprecision(2) << distance << " meters far : " << std::setprecision(8);
    std::cout << "(" << bg::get<0>(found_node) << ";" << bg::get<1>(found_node) << ")";
    std::cout << "\n\n";

    std::cout << "This node has id " << node_id << " : ";
    std::cout << "https://www.openstreetmap.org/node/" << node_id << "\n\n";

    if (static_cast<float>(distance) > MAX_ALLOWED_DISTANCE_IN_M) {
        std::cerr << "ERROR : nearest node seems further than the max allowed distance = " << MAX_ALLOWED_DISTANCE_IN_M
                  << " meters.\n";
        std::exit(4);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " OSMFILE  LONG  LAT\n";
        std::exit(1);
    }

    char const* const osmfile = argv[1];
    double requested_longitude = std::stod(argv[2]);
    double requested_latitude = std::stod(argv[3]);
    const Point requested_point = Point{requested_longitude, requested_latitude};
    constexpr float const MAX_ALLOWED_DISTANCE_IN_M = 40;
    std::cout << std::setprecision(8);
    std::cout << "Querying nearest node to point : " << requested_longitude << ";" << requested_latitude << std::endl;
    std::cout << "Loading file : " << osmfile << std::endl;

    RTree rtree = build_rtree_from_osm_data(osmfile);
    auto nearest = find_nearest(rtree, requested_point);
    display(nearest, MAX_ALLOWED_DISTANCE_IN_M);

    return 0;
}

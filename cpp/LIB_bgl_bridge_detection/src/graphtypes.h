#pragma once

#include <string>
#include <vector>
#include <osmium/osm/location.hpp>
#include <osmium/osm/types.hpp>

#include <boost/geometry.hpp>
#include <boost/graph/adjacency_list.hpp>

using Point = boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>;
using MultiPoint = boost::geometry::model::multi_point<Point>;
using Polygon = boost::geometry::model::polygon<Point>;

using NodeOsmId = osmium::object_id_type;
using WayId = osmium::object_id_type;
using LocatedNode = std::pair<NodeOsmId, osmium::Location>;
using Polyline = std::vector<osmium::Location>;


inline std::string node_url(NodeOsmId id) {
    return std::string("https://www.openstreetmap.org/node/") + std::to_string(id);
}

// boost::graph :
struct VertexProperty {
    double latitude;
    double longitude;
    NodeOsmId osmid;
};

struct EdgeProperty {
    float length_m;
    Polyline geometry;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty>;
using VertexDescriptor = boost::graph_traits<Graph>::vertex_descriptor;
using EdgeDescriptor = boost::graph_traits<Graph>::edge_descriptor;
using EdgeIterator = boost::graph_traits<Graph>::edge_iterator;

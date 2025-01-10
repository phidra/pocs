#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/osm/types.hpp>
#include <string>
#include <vector>

using NodeOsmId = osmium::object_id_type;
using WayId = osmium::object_id_type;
using LocatedNode = std::pair<NodeOsmId, osmium::Location>;
using Polyline = std::vector<osmium::Location>;

struct Node {
    inline Node(NodeOsmId osm_id_, osmium::Location const& location_) : id{osm_id_}, location{location_} {}

    inline double lon() const { return location.lon(); }
    inline double lat() const { return location.lat(); }

    inline bool operator==(Node const& other) const { return this->id == other.id; }  // needed by set<T>

    NodeOsmId id;
    osmium::Location location;
};

inline std::string node_url(NodeOsmId id) {
    return std::string("https://www.openstreetmap.org/node/") + std::to_string(id);
}
inline std::string node_url(Node n) {
    return node_url(n.id);
}

struct NodeHasher {
    size_t operator()(Node const& n) const { return std::hash<NodeOsmId>{}(n.id); }
};

// NOTE : a single OSM way can be splitted in several edges.
struct Edge {
    inline Edge(NodeOsmId node_from_, NodeOsmId node_to_, Polyline&& geometry_, float length_m_, float weight_) :
        node_from{node_from_, geometry_.front()},
        node_to{node_to_, geometry_.back()},
        length_m{length_m_},
        geometry{geometry_} {}

    Node node_from;
    Node node_to;
    float length_m;
    Polyline geometry;
};

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

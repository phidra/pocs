#pragma once

#include <osmium/osm/types.hpp>

struct Edge {
    inline Edge(osmium::object_id_type node_from_, osmium::object_id_type node_to_, double length_m_)
        : node_from{node_from_}, node_to{node_to_}, length_m{length_m_} {}
    osmium::object_id_type node_from;
    osmium::object_id_type node_to;
    double length_m;
};

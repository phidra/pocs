#pragma once

#include <vector>
#include <map>
#include <osmium/handler.hpp>

#include "common/graphtypes.h"
#include "common/polygon.h"

static BgPolygon DEFAULT_POLYGON{};  // by default = no polygon

// libosmium Handler that fills way+nodes structures
struct FillingHandler : public osmium::handler::Handler {
    std::map<WayId, std::vector<LocatedNode> > way_to_nodes;  // for a given way, stores its nodes
    std::map<NodeOsmId, int> node_use_counter;                // for a given node, counts how many ways use it
    // NOTE : why is it useful to count the numeber of ways that use a given node ?
    // Basically, to allow the use of edges in routing algorithms.
    // In OSM data, there can be wome ways like this :
    //
    // A----------(x)-----------B
    //             |
    //             |
    //             C
    // Here, there are only TWO ways : [AB] and [xC] !
    // If we only add TWO edges in the graph (AB) and (xC), it will NOT be possible to go from A to C
    // Thus, we have to split the way AB in two edges Ax and xB.
    // The simplest way to do this, is to track the ways that use a given node (here, x).
    // We can then add an edge, each time a node is used by 2 different ways.

    BgPolygon polygon;                                        // the ways outside of this polygon will be ignored
    inline FillingHandler(BgPolygon polygon_ = DEFAULT_POLYGON) : polygon(polygon_) {}
    void way(const osmium::Way& way) noexcept;
};

bool is_way_interesting(const osmium::Way& way);
bool is_way_in_polygon(const osmium::Way& way, const BgPolygon& polygon);

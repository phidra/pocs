#pragma once

#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include "graphtypes.h"

namespace bg = boost::geometry;

using Point = bg::model::point<double, 2, bg::cs::cartesian>;
using MultiPoint = bg::model::multi_point<Point>;
using Polygon = bg::model::polygon<Point>;

std::pair<MultiPoint, Polygon> build_convex_hull(Graph const& graph,
                                                 std::vector<VertexDescriptor> const& connected_component) {
    MultiPoint points;
    for (auto vd : connected_component) {
        bg::append(points, Point(graph[vd].latitude, graph[vd].longitude));
    }
    Polygon hull;
    bg::convex_hull(points, hull);

    return {points, hull};
}

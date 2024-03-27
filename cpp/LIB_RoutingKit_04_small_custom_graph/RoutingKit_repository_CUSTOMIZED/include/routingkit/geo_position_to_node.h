#ifndef ROUTING_KIT_GEO_POSITION_TO_NODE_H
#define ROUTING_KIT_GEO_POSITION_TO_NODE_H

#include <routingkit/constants.h>

#include <algorithm>
#include <vector>

namespace RoutingKit {

class GeoPositionToNode {
   public:
    GeoPositionToNode(){};

    GeoPositionToNode(std::vector<float> const& latitude, std::vector<float> const& longitude);

    unsigned point_count() const { return point_position.size(); }

    struct NearestNeighborhoodQueryResult {
        unsigned id;
        float distance;
    };

    // query_radius is in meter
    NearestNeighborhoodQueryResult find_nearest_neighbor_within_radius(float query_latitude,
                                                                       float query_longitude,
                                                                       float query_radius) const;
    std::vector<GeoPositionToNode::NearestNeighborhoodQueryResult>
    find_all_nodes_within_radius(float query_latitude, float query_longitude, float query_radius) const;

    // private:
    struct PointPosition {
        float latitude;
        float longitude;
    };

    std::vector<PointPosition> point_position;
    std::vector<unsigned> point_id;
};

}  // namespace RoutingKit

#endif

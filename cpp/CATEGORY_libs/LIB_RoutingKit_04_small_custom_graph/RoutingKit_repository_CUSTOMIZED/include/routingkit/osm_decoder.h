#ifndef OSM_DECODER_H
#define OSM_DECODER_H

#include <routingkit/tag_map.h>
#include <stdint.h>

#include <functional>
#include <string>
#include <vector>

namespace RoutingKit {

enum class OSMIDType { node, way, relation };

struct OSMRelationMember {
    OSMIDType type;
    uint64_t id;
    char const* role;
};

void unordered_read_osm_pbf(
    std::string const& file_name,
    std::function<void(uint64_t osm_node_id, double latitude, double longitude, TagMap const& tags)> node_callback,
    std::function<void(uint64_t osm_way_id, std::vector<uint64_t> const& osm_node_id_list, TagMap const& tags)>
        way_callback,
    std::function<void(uint64_t osm_relation_id, std::vector<OSMRelationMember> const& member_list, TagMap const& tags)>
        relation_callback,
    std::function<void(std::string const& msg)> log_message = [](std::string const&) {});

void ordered_read_osm_pbf(
    std::string const& file_name,
    std::function<void(uint64_t osm_node_id, double latitude, double longitude, TagMap const& tags)> node_callback,
    std::function<void(uint64_t osm_way_id, std::vector<uint64_t> const& osm_node_id_list, TagMap const& tags)>
        way_callback,
    std::function<void(uint64_t osm_relation_id, std::vector<OSMRelationMember> const& member_list, TagMap const& tags)>
        relation_callback,
    std::function<void(std::string const& msg)> log_message = [](std::string const&) {},
    bool file_is_ordered_even_though_file_header_says_that_it_is_unordered = false);

void speedtest_osm_pbf_reading(std::string const& pbf_file, std::function<void(std::string)> log_message);

}  // namespace RoutingKit

#endif

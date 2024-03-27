#pragma once

#include <vector>
#include <ostream>
#include <fstream>
#include <iomanip>

#include "graphtypes.h"
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

template <typename GraphType>
struct Traits {};

// Pour assurer que le process de dump est identique entre vector<Edge> ou boost::Graph,
// la fonction de dump est templatisée, de sorte que la même fonction soit appliquée aux deux types.
// Pour que ça marche, il faut définir des traits renseignants comment itérer sur un graphe.

template <>
struct Traits<std::vector<Edge>> {
    using GraphType = std::vector<Edge>;
    using EdgeIterator = std::vector<Edge>::const_iterator;
    static std::pair<EdgeIterator, EdgeIterator> get_iterators(GraphType const& v) { return {v.begin(), v.end()}; }
    static Polyline get_geometry(GraphType const&, EdgeIterator ite) { return ite->geometry; }
    static NodeOsmId get_node_from_id(GraphType const&, EdgeIterator ite) { return ite->node_from.id; }
    static NodeOsmId get_node_to_id(GraphType const&, EdgeIterator ite) { return ite->node_to.id; }
    static float get_length_m(GraphType const&, EdgeIterator ite) { return ite->length_m; }
};

template <>
struct Traits<Graph> {
    using GraphType = Graph;
    using EdgeIterator = boost::graph_traits<Graph>::edge_iterator;
    static std::pair<EdgeIterator, EdgeIterator> get_iterators(Graph const& graph) { return boost::edges(graph); }
    static Polyline get_geometry(GraphType const& graph, EdgeIterator ite) { return graph[*ite].geometry; }
    static NodeOsmId get_node_from_id(GraphType const& graph, EdgeIterator ite) {
        return graph[boost::source(*ite, graph)].osmid;
    }
    static NodeOsmId get_node_to_id(GraphType const& graph, EdgeIterator ite) {
        return graph[boost::target(*ite, graph)].osmid;
    }
    static float get_length_m(GraphType const& graph, EdgeIterator ite) { return graph[*ite].length_m; }
};

template <typename GraphType>
void dump_geojson_graph(std::ostream& out, GraphType const& graph) {
    // EXPECTED OUTPUT :
    // {
    //     "type": "FeatureCollection",
    //     "features": [
    //         {
    //             "type": "Feature",
    //             "geometry": {
    //                 "type": "LineString",
    //                 "coordinates": [
    //                     [
    //                         7.4259518,
    //                         43.7389494
    //                     ],
    //                     [
    //                         7.4258602,
    //                         43.7389997
    //                     ]
    //                 ]
    //             },
    //             "properties": {
    //                 "node_from": "https://www.openstreetmap.org/node/21912089",
    //                 "node_to": "https://www.openstreetmap.org/node/7265761724",
    //                 "node_from_url": "https://www.openstreetmap.org/node/21912089",
    //                 "node_to_url": "https://www.openstreetmap.org/node/7265761724",
    //                 "length_meters": 9.245828628540039
    //             }
    //         },
    //         ... other features ...
    //     ]
    // }

    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    rapidjson::Value features(rapidjson::kArrayType);

    using T = Traits<GraphType>;

    typename T::EdgeIterator edge, edge_end;
    for (std::tie(edge, edge_end) = T::get_iterators(graph); edge != edge_end; ++edge) {
        // coordinates :
        rapidjson::Value coordinates(rapidjson::kArrayType);

        auto edge_geometry = T::get_geometry(graph, edge);
        auto node_from_id = T::get_node_from_id(graph, edge);
        auto node_from_url = node_url(node_from_id);
        auto node_to_id = T::get_node_to_id(graph, edge);
        auto node_to_url = node_url(node_to_id);
        auto length_m = T::get_length_m(graph, edge);

        for (auto& node_location : edge_geometry) {
            rapidjson::Value loc(rapidjson::kArrayType);
            loc.PushBack(rapidjson::Value().SetDouble(node_location.lon()), a);
            loc.PushBack(rapidjson::Value().SetDouble(node_location.lat()), a);
            coordinates.PushBack(loc, a);
        }

        // properties :
        rapidjson::Value properties(rapidjson::kObjectType);
        properties.AddMember("node_from", rapidjson::Value(node_from_id), a);
        properties.AddMember("node_to", rapidjson::Value(node_to_id), a);
        properties.AddMember("node_from_url", rapidjson::Value().SetString(node_from_url.c_str(), a), a);
        properties.AddMember("node_to_url", rapidjson::Value().SetString(node_to_url.c_str(), a), a);
        properties.AddMember("length_meters", length_m, a);

        // geometry :
        rapidjson::Value geometry(rapidjson::kObjectType);
        geometry.AddMember("type", "LineString", a);
        geometry.AddMember("coordinates", coordinates, a);

        // feature :
        rapidjson::Value feature(rapidjson::kObjectType);
        feature.AddMember("type", "Feature", a);
        feature.AddMember("geometry", geometry, a);
        feature.AddMember("properties", properties, a);
        features.PushBack(feature, a);
    }

    doc.AddMember("features", features, a);

    // dumping :
    rapidjson::OStreamWrapper out_wrapper(out);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(out_wrapper);
    doc.Accept(writer);
}

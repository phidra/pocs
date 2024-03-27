#pragma once

#include <vector>
#include <ostream>
#include <fstream>
#include <iomanip>

#include "graphtypes.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include <boost/geometry.hpp>

inline void dump_geojson_graph(std::ostream& out, Graph const& graph) {
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

    boost::graph_traits<Graph>::edge_iterator edge, edge_end;
    for (std::tie(edge, edge_end) = boost::edges(graph); edge != edge_end; ++edge) {
        // coordinates :
        rapidjson::Value coordinates(rapidjson::kArrayType);

        auto edge_geometry = graph[*edge].geometry;
        auto node_from_id = graph[boost::source(*edge, graph)].osmid;
        auto node_from_url = node_url(node_from_id);
        auto node_to_id = graph[boost::target(*edge, graph)].osmid;
        auto node_to_url = node_url(node_to_id);
        auto length_m = graph[*edge].length_m;

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

template <typename Polylines>
void geometries_to_geojson_linestrings(std::ostream& out,
                                       Polylines const& polylines,
                                       const std::string stroke_color = "#555555",
                                       const int stroke_width = 2) {
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    rapidjson::Value features(rapidjson::kArrayType);

    for (auto polyline = boost::begin(polylines); polyline != boost::end(polylines); ++polyline) {
        // coordinates :
        rapidjson::Value coordinates(rapidjson::kArrayType);
        for (auto& node_location : *polyline) {
            rapidjson::Value loc(rapidjson::kArrayType);
            loc.PushBack(rapidjson::Value().SetDouble(node_location.lon()), a);
            loc.PushBack(rapidjson::Value().SetDouble(node_location.lat()), a);
            coordinates.PushBack(loc, a);
        }

        // properties :
        rapidjson::Value properties(rapidjson::kObjectType);

        // geometry :
        rapidjson::Value geometry(rapidjson::kObjectType);
        geometry.AddMember("coordinates", coordinates, a);
        geometry.AddMember("type", "LineString", a);

        // feature :
        rapidjson::Value feature(rapidjson::kObjectType);
        feature.AddMember("type", "Feature", a);
        feature.AddMember("geometry", geometry, a);

        properties.AddMember("stroke-width", stroke_width, a);
        properties.AddMember("stroke-opacity", 1, a);
        properties.AddMember("stroke", rapidjson::Value().SetString(stroke_color.c_str(), a), a);
        feature.AddMember("properties", properties, a);

        features.PushBack(feature, a);
    }

    doc.AddMember("features", features, a);

    // dumping :
    rapidjson::OStreamWrapper out_wrapper(out);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(out_wrapper);
    doc.Accept(writer);
    out << std::endl;
}

template <typename Edges>
void edges_to_geojson_linestrings(std::ostream& out,
                                  Edges const& edges,
                                  Graph const& graph,
                                  const std::string stroke_color = "#555555",
                                  const int stroke_width = 2) {
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    rapidjson::Value features(rapidjson::kArrayType);

    for (auto edge = boost::begin(edges); edge != boost::end(edges); ++edge) {
        // coordinates :
        rapidjson::Value coordinates(rapidjson::kArrayType);

        auto edge_geometry = graph[*edge].geometry;
        auto node_from_id = graph[boost::source(*edge, graph)].osmid;
        auto node_from_url = node_url(node_from_id);
        auto node_to_id = graph[boost::target(*edge, graph)].osmid;
        auto node_to_url = node_url(node_to_id);
        auto length_m = graph[*edge].length_m;

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
        geometry.AddMember("coordinates", coordinates, a);
        geometry.AddMember("type", "LineString", a);

        // feature :
        rapidjson::Value feature(rapidjson::kObjectType);
        feature.AddMember("type", "Feature", a);
        feature.AddMember("geometry", geometry, a);

        properties.AddMember("stroke-width", stroke_width, a);
        properties.AddMember("stroke-opacity", 1, a);
        properties.AddMember("stroke", rapidjson::Value().SetString(stroke_color.c_str(), a), a);
        feature.AddMember("properties", properties, a);

        features.PushBack(feature, a);
    }

    doc.AddMember("features", features, a);

    // dumping :
    rapidjson::OStreamWrapper out_wrapper(out);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(out_wrapper);
    doc.Accept(writer);
    out << std::endl;
}

template <typename Points>
void points_to_geojson_linestring(std::ostream& out, Points const& points) {
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    // coordinates :
    rapidjson::Value coordinates(rapidjson::kArrayType);

    for (auto it = boost::begin(points); it != boost::end(points); ++it) {
        double lat = boost::geometry::get<0>(*it);
        double lng = boost::geometry::get<1>(*it);
        rapidjson::Value coords(rapidjson::kArrayType);
        coords.PushBack(rapidjson::Value(lng), a);
        coords.PushBack(rapidjson::Value(lat), a);
        coordinates.PushBack(coords, a);
    }

    // geometry :
    rapidjson::Value geometry(rapidjson::kObjectType);
    geometry.AddMember("coordinates", coordinates, a);
    geometry.AddMember("type", "LineString", a);

    // properties :
    rapidjson::Value properties(rapidjson::kObjectType);
    properties.AddMember("nb_points", rapidjson::Value(boost::geometry::num_points(points)), a);

    // feature :
    rapidjson::Value feature(rapidjson::kObjectType);
    feature.AddMember("type", "Feature", a);
    feature.AddMember("geometry", geometry, a);
    feature.AddMember("properties", properties, a);

    rapidjson::Value features(rapidjson::kArrayType);
    features.PushBack(feature, a);
    doc.AddMember("features", features, a);

    // dumping :
    rapidjson::OStreamWrapper out_wrapper(out);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(out_wrapper);
    doc.Accept(writer);
    out << std::endl;
}

template <typename Points>
void points_to_geojson_points(std::ostream& out, Points const& points) {
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    rapidjson::Value features(rapidjson::kArrayType);

    for (auto const& point : points) {
        double lat = boost::geometry::get<0>(point);
        double lng = boost::geometry::get<1>(point);

        // coordinates :
        rapidjson::Value coordinates(rapidjson::kArrayType);
        coordinates.PushBack(rapidjson::Value().SetDouble(lng), a);
        coordinates.PushBack(rapidjson::Value().SetDouble(lat), a);

        // geometry :
        rapidjson::Value geometry(rapidjson::kObjectType);
        geometry.AddMember("coordinates", coordinates, a);
        geometry.AddMember("type", "Point", a);

        // properties :
        rapidjson::Value properties(rapidjson::kObjectType);

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
    out << std::endl;
}

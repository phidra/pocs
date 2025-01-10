#include "geojson.h"

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <fstream>
#include <iomanip>

using namespace std;

void dump_geojson_graph(ostream& out, vector<Edge> const& edges) {
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
    //                 "is_stop_node_from": false,
    //                 "is_stop_node_to": false,
    //                 "weight": 7.081911563873291,
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
    for (auto& edge : edges) {
        // coordinates :
        rapidjson::Value coordinates(rapidjson::kArrayType);
        for (auto& node_location : edge.geometry) {
            rapidjson::Value loc(rapidjson::kArrayType);
            loc.PushBack(rapidjson::Value().SetDouble(node_location.lon()), a);
            loc.PushBack(rapidjson::Value().SetDouble(node_location.lat()), a);
            coordinates.PushBack(loc, a);
        }

        // properties :
        rapidjson::Value properties(rapidjson::kObjectType);
        properties.AddMember("node_from", rapidjson::Value().SetString(edge.node_from.id.c_str(), a), a);
        properties.AddMember("node_to", rapidjson::Value().SetString(edge.node_to.id.c_str(), a), a);
        properties.AddMember("node_from_url", rapidjson::Value().SetString(edge.node_from.url.c_str(), a), a);
        properties.AddMember("node_to_url", rapidjson::Value().SetString(edge.node_to.url.c_str(), a), a);
        properties.AddMember("is_stop_node_from", edge.node_from.is_stop, a);
        properties.AddMember("is_stop_node_to", edge.node_to.is_stop, a);
        properties.AddMember("weight", edge.weight, a);
        properties.AddMember("length_meters", edge.length_m, a);

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

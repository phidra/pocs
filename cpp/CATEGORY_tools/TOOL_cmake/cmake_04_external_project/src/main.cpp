#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"

void dump_geojson(std::ostream& out) {
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    rapidjson::Value features(rapidjson::kArrayType);

    // coordinates :
    rapidjson::Value coordinates(rapidjson::kArrayType);
    coordinates.PushBack(2.294436693191528, a);
    coordinates.PushBack(48.85827053088322, a);

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

    doc.AddMember("features", features, a);

    // dumping :
    rapidjson::OStreamWrapper out_wrapper(out);
    rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(out_wrapper);
    doc.Accept(writer);
}

int main(int argc, char* argv[]) {
    std::cout << std::endl;
    std::cout << "Et voici un geojson tout frais à afficher :-)" << std::endl;
    std::cout << std::endl;
    dump_geojson(std::cout);
    std::cout << std::endl;
    std::cout << std::endl;
    return 0;
}

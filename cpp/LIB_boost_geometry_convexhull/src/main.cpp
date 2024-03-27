#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

using namespace std;
namespace bg = boost::geometry;

using Point = bg::model::point<double, 2, bg::cs::cartesian>;
using MultiPoint = bg::model::multi_point<Point>;
using Polygon = bg::model::polygon<Point>;

template <typename Points>
void to_geojson(ostream& out, Points const& points) {
    rapidjson::Document doc(rapidjson::kObjectType);
    rapidjson::Document::AllocatorType& a = doc.GetAllocator();
    doc.AddMember("type", "FeatureCollection", a);

    rapidjson::Value features(rapidjson::kArrayType);

    // coordinates :
    rapidjson::Value coordinates(rapidjson::kArrayType);

    for (auto it = boost::begin(points); it != boost::end(points); ++it) {
        double lng = bg::get<0>(*it);
        double lat = bg::get<1>(*it);
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
    properties.AddMember("nb_points", rapidjson::Value(bg::num_points(points)), a);

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

    out << endl;
}

int main(void) {
    cout << setprecision(15);
    MultiPoint points;
    bg::append(points, Point{2.339315414428711, 48.87132047236496});
    bg::append(points, Point{2.3177719116210938, 48.870868827622395});
    bg::append(points, Point{2.338714599609375, 48.86714260293412});
    bg::append(points, Point{2.335023880004883, 48.84856379804802});
    bg::append(points, Point{2.3438644409179688, 48.86742490239797});
    bg::append(points, Point{2.3660945892333984, 48.86578754333881});
    bg::append(points, Point{2.3441219329833984, 48.87132047236496});
    bg::append(points, Point{2.3448944091796875, 48.88249738062104});

    cout << "POINTS :" << endl;
    to_geojson(cout, points);
    cout << endl;

    Polygon hull;
    bg::convex_hull(points, hull);

    cout << "HULL :" << endl;
    to_geojson(cout, hull.outer());
    cout << endl;

    cerr << "All is ok." << endl;
    return 0;
}

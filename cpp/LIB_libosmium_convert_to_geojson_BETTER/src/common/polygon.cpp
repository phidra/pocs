#include "polygon.h"

#include <iostream>

using namespace std;

BgPolygon create_polygon(vector<pair<double, double>> const& points) {
    // precondition = les points doivent être définis dans le sens anti-horaire
    // precondition = le polygon doit être fermé (= le dernier point et le premier point sont identiques)
    // cf. https://www.boost.org/doc/libs/1_74_0/libs/geometry/doc/html/geometry/reference/models/model_polygon.html
    BgPolygon polygon;
    for (auto& point : points) {
        double lon = point.first;
        double lat = point.second;
        boost::geometry::append(polygon.outer(), BgPoint(lon, lat));
    }
    return polygon;
}

bool is_inside(BgPolygon const& polygon, double lon, double lat) {
    BgPoint point{lon, lat};
    return boost::geometry::within(point, polygon);
}

bool is_empty(BgPolygon const& polygon) {
    return polygon.outer().empty();
}

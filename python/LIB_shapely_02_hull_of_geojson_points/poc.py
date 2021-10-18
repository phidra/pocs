#!/usr/bin/env python3

import json

from shapely.geometry import MultiPoint, shape, LinearRing, box, Polygon


def main():
    print(
        """
    DESCRIPTION = loads a set of points from a geojson file.

    Displays hull, and bbox of the points, as geojson.
    """
    )
    multipoint = parse_multipoint("points.geojson")

    print("=== HULL IS :")
    hull = multipoint.convex_hull
    print(to_geojson(hull.exterior))
    print("")

    print("=== BBOX IS :")
    bbox = multipoint.envelope
    print(to_geojson(bbox.exterior))
    print("")

    print("=== EXTENDED BBOX IS :")
    extended_bbox = bbox_plus_ten_percent(bbox)
    print(to_geojson(extended_bbox.exterior))
    print("")


def bbox_plus_ten_percent(geom) -> Polygon:
    min_x, min_y, max_x, max_y = geom.bounds

    delta_x = max_x - min_x
    ten_percent_x = delta_x / 10
    extended_min_x = min_x - ten_percent_x
    extended_max_x = max_x + ten_percent_x

    delta_y = max_y - min_y
    ten_percent_y = delta_y / 10
    extended_min_y = min_y - ten_percent_y
    extended_max_y = max_y + ten_percent_y
    ten_percent_y = delta_y / 10

    return box(extended_min_x, extended_min_y, extended_max_x, extended_max_y)


def parse_multipoint(filename: str) -> MultiPoint:
    with open("points.geojson", "r") as f:
        data = json.load(f)
    points = []
    for feature in data["features"]:
        parsed_point = shape(feature["geometry"])
        points.append(parsed_point)
    return MultiPoint(points)


def to_geojson(polygon_exterior: LinearRing) -> str:
    polygon_coords = []
    for x, y in polygon_exterior.coords:
        polygon_coords.append("[{}, {}]".format(x, y))
    polygon_coords_str = ",".join(polygon_coords)

    BEFORE = """
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "type": "Polygon",
        "coordinates": [
            [
    """
    AFTER = """
            ]
            ]
          }
        }
      ]
    }
    """

    return BEFORE + polygon_coords_str + AFTER


if __name__ == "__main__":
    main()

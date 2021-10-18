#!/usr/bin/env python3

from shapely.geometry import Point, LineString, LinearRing, Polygon, MultiPoint


NOTES = """

github      = https://github.com/Toblerity/Shapely
doc         = https://shapely.readthedocs.io/en/latest/index.html
user-manual = https://shapely.readthedocs.io/en/latest/manual.html

TIP : to search the manual, prefix with `object.` (e.g. `object.convex_hull`)

Notes on base forms :
- there are 3 base objects : POINT, LINESTRING, POLYGON
- coords always returns a LIST of tuples (for the point, this list contains a single element)
- representative_point is guaranteed to belong to the shape (thus it might NOT be the centroid)
- for LinearRings with final point != first point, an additional point (identical to first-point) is added
- polygon coords cannot be directly used (because of possible holes), one must use interiors/exterior
- there are no "coords" attribute on a Multipoint


Useful things (not all of them illustrated) :
- easily construct rectangular polygons with box :
    https://shapely.readthedocs.io/en/latest/manual.html#shapely.geometry.box
- boundary of a shape :
    https://shapely.readthedocs.io/en/latest/manual.html#object.boundary
- interpolate (project) = find the point at "this" distance of the extremity of the line (or the opposite)
    https://shapely.readthedocs.io/en/latest/manual.html#object.interpolate
    https://shapely.readthedocs.io/en/latest/manual.html#object.project
- centroid :
    https://shapely.readthedocs.io/en/latest/manual.html#object.centroid
- unary predicates : is_empty / is_ring / is_simple / is_valid :
    https://shapely.readthedocs.io/en/latest/manual.html#unary-predicates
- binary predicates : equals / almost_equals / contains / within / intersects / overlaps / ...
    https://shapely.readthedocs.io/en/latest/manual.html#binary-predicates
- set operations : difference / intersection / union / ...
    https://shapely.readthedocs.io/en/latest/manual.html#set-theoretic-methods
- many more useful things :
    simplify
    affine transformations
    line to polygon / line merge
    delaunay / voronoi
    rtree building and querying
    morphological dilation/erosion
    minimum_rotated_rectangle

"""


def base_properties_of_different_shapes():

    # POINT :
    p1 = Point(42.5, -3.1416)
    print("")
    print(p1)
    print("p1 area              = ", p1.area)
    print("p1 length            = ", p1.length)
    print("p1 bounds            = ", p1.bounds)
    print("p1 x                 = ", p1.x)
    print("p1 y                 = ", p1.y)
    print("p1 coords            = ", list(p1.coords))
    print("geom type            = ", repr(p1.geom_type))
    print("representative point = ", p1.representative_point())
    print("centroid             = ", p1.centroid)
    print("is centroid on shape = ", p1.centroid.within(p1))

    # POLYLINE :
    p2 = Point(10, 20)
    p3 = Point(100, 200)
    line = LineString([p1, p2, p3])
    print("")
    print(line)
    print("line area                = ", line.area)
    print("line length              = ", line.length)
    print("line bounds              = ", line.bounds)
    print("line coords              = ", list(line.coords))
    print("number of points in line = ", len(line.coords))
    print("geom type                = ", repr(line.geom_type))
    print("representative point     = ", line.representative_point())
    print("centroid                 = ", line.centroid)
    print("is centroid on shape     = ", line.centroid.within(line))

    # LINEAR RING is a special polyline with first-point == last-point :
    linear_explicit = LinearRing([p1, p2, p3, p1])  # explicit = last-point explicitly equals first-point
    print("")
    print(linear_explicit)
    print("linear_explicit area                = ", linear_explicit.area)
    print("linear_explicit length              = ", linear_explicit.length)
    print("linear_explicit bounds              = ", linear_explicit.bounds)
    print("linear_explicit coords              = ", list(linear_explicit.coords))
    print("number of points in linear_explicit = ", len(linear_explicit.coords))
    print("geom type                           = ", repr(linear_explicit.geom_type))
    print("representative point                = ", linear_explicit.representative_point())
    print("centroid                            = ", linear_explicit.centroid)
    print("is centroid on shape                = ", linear_explicit.centroid.within(linear_explicit))

    # warning : if LinearRing final-point is implicit, an additional point is added !
    linear_implicit = LinearRing([p1, p2, p3])  # implicit = last-point != first-point, length will STILL be 4 !
    print("length of linear ring EXPLICIT = ", len(linear_explicit.coords))  # explicitly 4
    print("length of linear ring IMPLICIT = ", len(linear_implicit.coords))  # implicitly 4 !!!
    assert len(linear_explicit.coords) == len(linear_implicit.coords)

    # POLYGON is constructed like LinearRings (but can also get holes, not illustrated here) :
    polygon = Polygon([p1, p2, p3, p1])  # explicit = last-point explicitly equals first-point
    print("")
    print(polygon)
    print("polygon area            = ", polygon.area)
    print("polygon length          = ", polygon.length)
    print("polygon bounds          = ", polygon.bounds)
    print("polygon EXTERIOR coords = ", list(polygon.exterior.coords))
    print("polygon's INTERIORS     = ", list(polygon.interiors))  # in our case : empty
    print("geom type               = ", repr(polygon.geom_type))
    print("representative point    = ", polygon.representative_point())
    print("centroid                = ", polygon.centroid)
    print("is centroid on shape    = ", polygon.centroid.within(polygon))
    print("")

    # MULTIPOINT
    multipoint = MultiPoint([p1, p2, p3])
    print("")
    print(multipoint)
    print("multipoint area                = ", multipoint.area)
    print("multipoint length              = ", multipoint.length)
    print("multipoint bounds              = ", multipoint.bounds)
    print("geom type                      = ", repr(multipoint.geom_type))
    print("representative point           = ", multipoint.representative_point())
    print("centroid                       = ", multipoint.centroid)
    print("is centroid on shape           = ", multipoint.centroid.within(multipoint))
    print("convex hull                    = ", multipoint.convex_hull)
    print("envelope (= rectangular hull)  = ", multipoint.envelope)


def various_relations_between_shapes():
    print("")
    print("VARIOUS RELATIONSHIPS BETWEEN SHAPES")

    # https://fr.wikipedia.org/wiki/Distance_de_Hausdorff
    line1 = LineString([(0, 0), (0, 10), (10, 10), (10, 0)])
    line2 = LineString([(0, 0), (0, 10), (11, 11), (10, 0)])
    line3 = LineString([(100, 100), (100, 1010), (1011, 1011), (1010, 100)])
    print("small hausdorff distance = ", line1.hausdorff_distance(line2))
    print("big   hausdorff distance = ", line1.hausdorff_distance(line3))

    # distance (= length of the projection ?)
    line = LineString([(-10, 0), (10, 0)])
    point = Point(0, 1)
    print("distance from point to line = ", line.distance(point))
    print("distance from line to point = ", point.distance(line))
    assert line.distance(point) == point.distance(line)

    # distance ALONG the line :
    print("points along the line :")
    print("at a distance of 5 of the origin = ", line.interpolate(5))
    print("at the third of the line         = ", line.interpolate(1 / 3, normalized=True))
    print("distance of (4,0) along the line = ", line.project(Point(4, 0)))

    # contains / within :
    polygon = Polygon([(0, 0), (0, 10), (10, 10), (10, 0)])
    point = Point(5, 5)
    print("is point in polygon A        = ", polygon.contains(point))
    print("is point in polygon B        = ", point.within(polygon))

    # set operations :
    other_polygon = Polygon([(5, 5), (5, 15), (15, 15), (15, 5)])
    print("polygon2 intersects polygon1 = ", polygon.intersects(other_polygon))
    print("difference                   = ", polygon.difference(other_polygon))
    print("intersection                 = ", polygon.intersection(other_polygon))
    print("union                        = ", polygon.union(other_polygon))


if __name__ == "__main__":
    print("POC DESCRIPTION = tests with shapely")
    base_properties_of_different_shapes()
    various_relations_between_shapes()

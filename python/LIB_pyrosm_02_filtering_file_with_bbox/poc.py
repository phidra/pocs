#!/usr/bin/env python3

import pyrosm
import matplotlib
from shapely.geometry import Polygon


def main():
    print("DESCRIPTION = loads pbf file, filtering either with rectangle or polygon")

    INPUT_PBF_FILE = "NOGIT_monaco.osm.pbf"

    # two ways of filtering input PBF file : bbox or polygon

    # with bbox :
    minx = 7.420191764831543
    maxx = 7.433924674987793
    miny = 43.732096232592276
    maxy = 43.738700949347866
    osm_with_rectangle = pyrosm.OSM(INPUT_PBF_FILE, bounding_box=[minx, miny, maxx, maxy])
    network_with_rectangle = osm_with_rectangle.get_network(network_type="driving")
    network_with_rectangle.plot()
    matplotlib.pyplot.title("BOUNDED BY RECTANGLE")
    matplotlib.pyplot.show()

    # with polygon :
    polygon = Polygon(
        [
            [7.427101135253906, 43.740189235644586],
            [7.421994209289551, 43.73950710901742],
            [7.417702674865722, 43.73752269645995],
            [7.416329383850098, 43.73470099661642],
            [7.417316436767578, 43.732592386855366],
            [7.424440383911132, 43.73113492201052],
            [7.427058219909668, 43.73175512415206],
            [7.4272727966308585, 43.732654405849246],
            [7.423667907714845, 43.73222027154342],
            [7.420449256896972, 43.73274743421963],
            [7.418389320373535, 43.73336761966254],
            [7.417531013488769, 43.734638979742],
            [7.417616844177245, 43.73587930502763],
            [7.418475151062011, 43.737336654410804],
            [7.420234680175781, 43.7382048456947],
            [7.4225521087646475, 43.7388869871581],
            [7.426028251647949, 43.739228054975506],
            [7.427401542663573, 43.73929006709723],
            [7.427101135253906, 43.740189235644586],
        ]
    )

    osm_with_polygon = pyrosm.OSM(INPUT_PBF_FILE, bounding_box=polygon)
    network_with_polygon = osm_with_polygon.get_network(network_type="driving")
    network_with_polygon.plot()
    matplotlib.pyplot.title("BOUNDED BY POLYGON")
    matplotlib.pyplot.show()


main()

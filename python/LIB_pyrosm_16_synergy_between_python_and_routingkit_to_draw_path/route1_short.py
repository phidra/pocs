#!/usr/bin/env python3


# https://www.openstreetmap.org/node/263077002
SOURCE_OSMID = 263077002

# https://www.openstreetmap.org/node/7781960118
TARGET_OSMID = 7781960118

# the shortest path between this source+target is expected to go through these nodes :
#       https://www.openstreetmap.org/node/263077002
#       https://www.openstreetmap.org/node/263077003
#       https://www.openstreetmap.org/node/5046302791
#       https://www.openstreetmap.org/node/5046302792
#       https://www.openstreetmap.org/node/7781960118

EXPECTED_SHORTEST_PATH_NODES = [
    263077002,
    263077003,
    5046302791,
    5046302792,
    7781960118,
]

# the shortest path between this source+target is expected to go through these ways :
#       https://www.openstreetmap.org/way/849737733
#       https://www.openstreetmap.org/way/849737754
#       https://www.openstreetmap.org/way/517029021
#       https://www.openstreetmap.org/way/849737736

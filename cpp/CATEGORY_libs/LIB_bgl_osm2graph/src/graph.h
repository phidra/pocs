#pragma once

#include <vector>

#include "graphtypes.h"
#include "polygon.h"

std::pair<std::vector<Edge>, Graph> osm_to_graph(std::string osmfile, BgPolygon polygon);

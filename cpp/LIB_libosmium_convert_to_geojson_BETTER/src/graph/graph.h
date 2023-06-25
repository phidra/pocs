#pragma once

#include <vector>

#include "common/graphtypes.h"
#include "common/polygon.h"

std::vector<Edge> osm_to_graph(std::string osmfile, BgPolygon polygon);

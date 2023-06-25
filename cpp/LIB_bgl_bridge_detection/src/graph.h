#pragma once

#include <vector>

#include "graphtypes.h"
#include "polygon.h"

Graph osm_to_graph(std::string osmfile, BgPolygon polygon);

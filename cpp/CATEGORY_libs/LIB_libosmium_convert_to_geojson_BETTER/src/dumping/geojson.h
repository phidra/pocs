#pragma once

#include <ostream>
#include <unordered_set>
#include <vector>

#include "common/graphtypes.h"

void dump_geojson_graph(std::ostream& out, std::vector<Edge> const& edges);

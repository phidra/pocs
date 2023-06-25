#pragma once

#include <vector>
#include <unordered_set>
#include <ostream>

#include "common/graphtypes.h"

void dump_geojson_graph(std::ostream& out, std::vector<Edge> const& edges);

#pragma once

#include <map>
#include <vector>
#include <string>
#include <osmium/osm/location.hpp>

#include "edge.h"

void create_dimacs(std::string output_dir,
                   std::map<int, osmium::Location> const& nodes,
                   std::vector<Edge> const& edges);

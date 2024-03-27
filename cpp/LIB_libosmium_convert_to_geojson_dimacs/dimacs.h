#pragma once

#include <map>
#include <osmium/osm/location.hpp>
#include <string>
#include <vector>

#include "edge.h"

void create_dimacs(std::string output_dir,
                   std::map<int, osmium::Location> const& nodes,
                   std::vector<Edge> const& edges);

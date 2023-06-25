#pragma once

#include <map>
#include <vector>
#include <string>
#include <osmium/osm/location.hpp>

void create_geojson(
    std::string output_dir,
    std::map<int, double> const & way_index_to_length,
    std::map<int, std::vector<osmium::Location> > const & way_index_to_geometry
);

#include <fstream>
#include <iomanip>

#include "geojson.h"

void create_geojson(
    std::string output_dir,
    std::map<int, double> const & way_index_to_length,
    std::map<int, std::vector<osmium::Location> > const & way_index_to_geometry
)
{
    // Expected output :
    // {
    //   "type": "Feature",
    //   "geometry": {
    //     "type": "LineString",
    //     "coordinates": [
    //       [2.2992761, 48.7527953],
    //       [2.2992149, 48.7528159]
    //     ]
    //   },
    //   "properties": {
    //     "highway": "path",
    //     "length_meters": 111.1234
    //   }
    // },

    std::ostringstream geojson_stream;

    geojson_stream << std::setprecision(8);
    // geojson header :
    std::string geojson_header = R"DELIM(
    {
        "type": "FeatureCollection",
            "features": [
    )DELIM";
    geojson_stream << geojson_header;


    for (auto& way: way_index_to_geometry)
    {
        auto const& way_locations = way.second;
        double length_m = way_index_to_length.at(way.first);

        // way header :
        geojson_stream << R"DELIM(
                {
                    "type": "Feature",
                    "geometry": {
                        "type": "LineString",
                        "coordinates": [
        )DELIM";

        // TODO : récupérer les nodes
        for (auto& node_location: way_locations)
        {
            geojson_stream << "\n[" << node_location.lon() << ", " << node_location.lat() << "],";
            // note : a comma is wrongfully added for the last node : it will be removed later.
        }

        // removing last comma :
        geojson_stream.seekp(-1, geojson_stream.cur);



        // way footer + properties :
        geojson_stream << "] },\n";
        geojson_stream << " \"properties\": { \n";
        geojson_stream << " \"length_meters\": " << length_m << " \n";  // no final comma
        geojson_stream << "}\n";
        geojson_stream << "},";
        // note : a comma is wrongfully added for the last way : it will be removed later.
    }

    // removing last comma :
    geojson_stream.seekp(-1, geojson_stream.cur);

    // geojson footer :
    std::string json_footer = R"DELIM(
        ]
    }
    )DELIM";
    geojson_stream << json_footer;

    // dumping :
    std::ofstream out(output_dir + "osm.geojson");
    out << geojson_stream.str();

}

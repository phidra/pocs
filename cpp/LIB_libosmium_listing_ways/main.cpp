#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <chrono>

#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/util/memory.hpp>
#include <osmium/visitor.hpp>
#include <osmium/index/map/sparse_mmap_array.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>

struct MyHandler : public osmium::handler::Handler {

    std::uint64_t total_nb_ways      = 0;
    std::uint64_t nb_highways      = 0;
    std::uint64_t nb_non_area      = 0;
    std::uint64_t nb_area      = 0;


    void way(const osmium::Way& w) noexcept {
        ++total_nb_ways;

        // displaying first 10 ways :
        if (total_nb_ways < 10)
        {
            std::cout << "WAY [" << w.id() << "] :" << std::endl;

            std::cout << "\tTAGS (" << w.tags().size() << ") :" << std::endl;
            for (auto const & t: w.tags())
            {
                std::cout << "\t\t" << t.key() << " -> " << t.value() << std::endl;
            }
            std::cout << "\tNODES (" << w.nodes().size() << ") :" << std::endl;
            for (auto const & n: w.nodes())
            {
                auto const & loc = n.location();
                if (loc.valid())
                {
                    std::cout << "\t\t" << n.location().lon() << " ; " << n.location().lat() << std::endl;
                }
            }
        }

        // counting highways :
        if (w.tags()["highway"] != nullptr)
        {
            ++nb_highways;
        }

        // counting (non-)areas ways :
        if (w.tags()["area"] == nullptr || strcmp(w.tags()["area"], "no") == 0)
        {
            ++nb_non_area;
        }
        else
        {
            ++nb_area;
            if (strcmp(w.tags()["area"], "yes") != 0)
            {
                std::cout << "AREA with curious tag : " << w.tags()["area"] << std::endl;
            }

        }



    }

};


int main(int argc, char* argv[]) {
    auto time_before = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " OSMFILE\n";
        std::exit(1);
    }

    try {


        auto interesting_types = osmium::osm_entity_bits::node | osmium::osm_entity_bits::way;
        osmium::io::Reader reader{argv[1], interesting_types};


        // location handler -> sette les locations des nodes des ways
        namespace map = osmium::index::map;
        using index_type = map::SparseMmapArray<osmium::unsigned_object_id_type, osmium::Location>;
        using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;
        index_type index;
        location_handler_type location_handler{index};

        MyHandler handler;
        osmium::apply(reader, location_handler, handler);
        reader.close();

        std::cout << "Ways: "      << handler.total_nb_ways << "\n";
        std::cout << "Highways: "      << handler.nb_highways << "\n";
        std::cout << "Areas: "      << handler.nb_area << "\n";
        std::cout << "Non-Areas: "      << handler.nb_non_area << "\n";
        std::cout << "Sum-Areas: "      << handler.nb_non_area + handler.nb_area << "\n";

        osmium::MemoryUsage memory;
        std::cout << "\nMemory used: " << memory.peak() << " MBytes\n";

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        std::exit(1);
    }
    auto time_after = std::chrono::steady_clock::now();
    auto duration = time_after - time_before;
    auto duration_in_s = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "POC = done in " << duration_in_s << " ms" << std::endl;
    return 0;
}


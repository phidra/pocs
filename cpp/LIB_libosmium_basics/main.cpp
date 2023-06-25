#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <chrono>

#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/util/memory.hpp>
#include <osmium/visitor.hpp>

// Handler derive from the osmium::handler::Handler base class. Usually you
// overwrite functions node(), way(), and relation(). Other functions are
// available, too. Read the API documentation for details.
struct CountHandler : public osmium::handler::Handler {

    std::uint64_t nodes     = 0;
    std::uint64_t ways      = 0;
    std::uint64_t relations = 0;

    // This callback is called by osmium::apply for each node in the data.
    void node(const osmium::Node&) noexcept {
        ++nodes;
    }

    // This callback is called by osmium::apply for each way in the data.
    void way(const osmium::Way&) noexcept {
        ++ways;
    }

    // This callback is called by osmium::apply for each relation in the data.
    void relation(const osmium::Relation&) noexcept {
        ++relations;
    }

}; // struct CountHandler


int main(int argc, char* argv[]) {
    std::cout << "POC d'un exemple fourni avec libosmium = charger le fichier et compter les éléments" << std::endl;
    auto time_before = std::chrono::steady_clock::now();

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " OSMFILE\n";
        std::exit(1);
    }

    try {
        // The Reader is initialized here with an osmium::io::File, but could
        // also be directly initialized with a file name.
        osmium::io::File input_file{argv[1]};
        osmium::io::Reader reader{input_file};

        // Create an instance of our own CountHandler and push the data from the
        // input file through it.
        CountHandler handler;
        osmium::apply(reader, handler);

        // You do not have to close the Reader explicitly, but because the
        // destructor can't throw, you will not see any errors otherwise.
        reader.close();

        std::cout << "Nodes: "     << handler.nodes << "\n";
        std::cout << "Ways: "      << handler.ways << "\n";
        std::cout << "Relations: " << handler.relations << "\n";

        // Because of the huge amount of OSM data, some Osmium-based programs
        // (though not this one) can use huge amounts of data. So checking actual
        // memore usage is often useful and can be done easily with this class.
        // (Currently only works on Linux, not OSX and Windows.)
        osmium::MemoryUsage memory;

        std::cout << "\nMemory used: " << memory.peak() << " MBytes\n";
    } catch (const std::exception& e) {
        // All exceptions used by the Osmium library derive from std::exception.
        std::cerr << e.what() << '\n';
        std::exit(1);
    }
    auto time_after = std::chrono::steady_clock::now();
    auto duration = time_after - time_before;
    auto duration_in_s = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << "POC = done in " << duration_in_s << " ms" << std::endl;
    return 0;
}


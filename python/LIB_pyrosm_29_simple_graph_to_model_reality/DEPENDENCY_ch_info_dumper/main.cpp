#include <routingkit/contraction_hierarchy.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "unpack.h"
#include "argparsing.h"
#include "dumping.h"

using namespace std;


string id_to_osm_node_url(long node_id) {
    return string("https://www.openstreetmap.org/node/") + to_string(node_id);
}


void usage(string prog_name) {
    cerr << "USAGE:" << endl;
    cerr << "\t" << prog_name << " <TAILS>  <HEADS>  <WEIGHTS>  <OSMIDS>  <OUTPUT-DIR>" << endl;
    cerr << endl;
    cerr << "Where  :" << endl;
    cerr << "\t TAILS / HEADS / WEIGHTS describe edges (as text files containing a single unsigned integer on each line)" << endl;
    cerr << "\t OSMIDS describes NODES (as text file containing a single osmid integer on each line)" << endl;
    cerr << "\t OUTPUT-DIR must point to an already existing dir" << endl;
    cerr << endl;
    cerr << "This program writes the CH infos on following files in OUTPUT-DIR :" << endl;
    cerr << "\t TODO" << endl;
    cerr << "\t TODO" << endl;
    cerr << "\t TODO" << endl;
    cerr << "\t TODO" << endl;
    cerr << "\t TODO" << endl;
    cerr << endl;
    cerr << "The path of the written files are written on stdout" << endl;
    cerr << endl;
}

RoutingKit::ContractionHierarchy build_ch(vector<unsigned> const& tails, vector<unsigned> const& heads, vector<unsigned> const& weights, size_t num_nodes) {
    auto ch = RoutingKit::ContractionHierarchy::build(
        num_nodes,
        tails,
        heads,
        weights
    );
    return ch;
}

int main(int argc, char* argv[]){

    string prog_name = argv[0];

    if (argc != 6) {
        cerr << endl;
        cerr << "WRONG NUMBER OF ARGS" << endl;
        usage(prog_name);
        exit(1);
    }

    auto tails_file = argv[1];
    auto heads_file = argv[2];
    auto weights_file = argv[3];
    auto osmids_file = argv[4];
    string output_dir = argv[5];
    if (output_dir.back() != '/') {
        output_dir.push_back('/');
    }

    cerr << "TAILS FILE      = " << tails_file << endl;
    cerr << "HEADS FILE      = " << heads_file << endl;
    cerr << "WEIGHTS FILE    = " << weights_file << endl;
    cerr << "OSMIDS FILE     = " << osmids_file << endl;
    cerr << "OUTPUT_DIR      = " << output_dir << std::endl;

    auto tails = get_vector_u(tails_file, prog_name);
    auto heads = get_vector_u(heads_file, prog_name);
    auto weights = get_vector_u(weights_file, prog_name);
    auto osmids = get_vector_l(osmids_file, prog_name);
    assert(tails.size() == heads.size());
    assert(tails.size() == weights.size());

    auto NUM_EDGES = tails.size();
    auto NUM_NODES = osmids.size();
    cerr << "NUM NODES    = " << NUM_NODES << endl;
    cerr << "NUM EDGES    = " << NUM_EDGES << endl;


    cerr << "Building CH" << endl;
    auto ch = build_ch(tails, heads, weights, NUM_NODES);


    cerr << "size or rank  = " << ch.rank.size() << endl;
    cerr << "size or order = " << ch.order.size() << endl;

    string rank_file = dump_vector(ch.rank,  output_dir + "NOGIT_OUT_ranks.txt");
    string order_file = dump_vector(ch.order,  output_dir + "NOGIT_OUT_orders.txt");
    cout << rank_file << endl;
    cout << order_file << endl;
    auto written_files_forward = dump_side(ch.forward, output_dir + "NOGIT_OUT_forward");
    for (auto f: written_files_forward) {
        cout << f << endl;
    }
    auto written_files_backward = dump_side(ch.backward, output_dir + "NOGIT_OUT_backward");
    for (auto f: written_files_backward) {
        cout << f << endl;
    }
}

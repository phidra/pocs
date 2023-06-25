#include <routingkit/contraction_hierarchy.h>
#include <iostream>
#include <fstream>


using namespace std;


void usage(string prog_name) {
    cerr << "USAGE:" << endl;
    cerr << "\t" << prog_name << " <TAILS>  <HEADS>  <WEIGHTS>  <NUM_NODES>  <SOURCE>  <TARGET>" << endl;
    cerr << endl;
    cerr << "Where  :" << endl;
    cerr << "\t TAILS / HEADS / WEIGHTS are text files that contains a single unsigned integer on each line" << endl;
    cerr << "\t NUM_NODES is an integer (number of nodes in the graph)" << endl;
    cerr << "\t SOURCE is the origin's node id (integer used in TAILS/HEADS)" << endl;
    cerr << "\t TARGET is the destination's node id (integer used in TAILS/HEADS)" << endl;
    cerr << endl;
    cerr << "This program writes the shortest path nodes on stdout on N+1 lines :" << endl;
    cerr << "\t LINE 1 = the length of the shortest path" << endl;
    cerr << "\t LINE 2 to N = the shortest path as newline-separated list of node ids" << endl;
    cerr << endl;
    cerr << "This program also writes human-useful information on stderr." << endl;
    cerr << endl;
}


int get_int(string int_arg, string prog_name) {
    int value = -1;
    try {
        value = stoi(int_arg);
    } catch (...) {
        cerr << "ERROR : unable to parse int value '" << int_arg << "'" << endl;
        usage(prog_name);
        exit(1);
    }
    return value;
}

vector<unsigned> get_vector(string filename, string prog_name) {
    ifstream stream{filename};
    if (!stream.good()) {
        cerr << "ERROR: unable to read : '" << filename << "'\n";
        cerr << "\n";
        exit(2);
    }
    string line;
    int value;
    vector<unsigned> to_return;
    while(getline(stream, line).good()) {
        value = stoi(line);
        if (value < 0) {
            cerr << "ERROR : bad parsed value (" << value << ") from file : " << filename << endl; 
            cerr << "\n";
            exit(3);
        }
        to_return.push_back(value);
    }
    return to_return;
}

RoutingKit::ContractionHierarchy build_ch(vector<unsigned> const& tails, vector<unsigned> const& heads, vector<unsigned> const& weights, int num_nodes) {
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

    if (argc != 7) {
        cerr << endl;
        cerr << "WRONG NUMBER OF ARGS" << endl;
        usage(prog_name);
        exit(1);
    }

    auto tails_file = argv[1];
    auto heads_file = argv[2];
    auto weights_file = argv[3];
    int NUM_NODES = get_int(argv[4], prog_name);
    int SOURCE = get_int(argv[5], prog_name);
    int TARGET = get_int(argv[6], prog_name);

    cerr << "TAILS FILE   = " << tails_file << endl;
    cerr << "HEADS FILE   = " << heads_file << endl;
    cerr << "WEIGHTS FILE = " << weights_file << endl;
    cerr << "NUM NODES    = " << NUM_NODES << endl;
    cerr << "SOURCE NODE  = " << SOURCE << endl;
    cerr << "TARGET NODE  = " << TARGET << endl;

    auto tails = get_vector(tails_file, prog_name);
    auto heads = get_vector(heads_file, prog_name);
    auto weights = get_vector(weights_file, prog_name);


    cerr << "Building CH" << endl;
    auto ch = build_ch(tails, heads, weights, NUM_NODES);
    cerr << "Computing shortest path between " << SOURCE << " and " << TARGET << endl;

    RoutingKit::ContractionHierarchyQuery ch_query(ch);
    ch_query.reset().add_source(SOURCE).add_target(TARGET).run();
    auto distance = ch_query.get_distance();
    auto path_as_nodes = ch_query.get_node_path();

    cerr << "To get from "<< SOURCE << " to "<< TARGET << " one needs " << distance << " meters." << endl;  // for humans
    cout << distance << endl;  // for automatic parsing
    cerr << "The path is";
    for(auto n: path_as_nodes) {
        cerr << " " << n << " ";  // for humans
        cout << n << endl;  // for automatic parsing
    }
    cerr << endl;
}

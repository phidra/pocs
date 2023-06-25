#include <routingkit/contraction_hierarchy.h>
#include <iostream>
#include <sstream>
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
    cerr << "This program writes its computations on stdout :" << endl;
    cerr << "\t LINE 1 = rank of each node (ordered by node id), as a space-separated list of ranks" << endl;
    cerr << "\t LINE 2 = the length of the shortest path" << endl;
    cerr << "\t LINE 3 = the shortest path as space-separated list of node ids" << endl;
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
    cerr << "The path is";
    for(auto n: path_as_nodes) {
        cerr << " " << n << " ";  // for humans
    }
    cerr << endl;


    // stdout line 1 = ranks of the nodes :
    ostringstream oss_rank;
    for (unsigned node_rkindex = 0; node_rkindex < NUM_NODES; ++node_rkindex) {
        oss_rank << ch.rank[node_rkindex] << " ";
    }
    auto oss_rank_str = oss_rank.str();
    cout << oss_rank_str.substr(0, oss_rank_str.size() - 1) << endl;  // remove final " "

    // stdout line 2 = shortest path length
    cout << distance << endl;

    // stdout line 3 = shortest path :
    ostringstream oss_path;
    for(auto n: path_as_nodes) {
        oss_path << n << " ";
    }
    auto oss_path_str = oss_path.str();
    cout << oss_path_str.substr(0, oss_path_str.size() - 1) << endl;  // remove final " "
}

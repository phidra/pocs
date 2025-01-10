#include <assert.h>
#include <routingkit/contraction_hierarchy.h>
#include <routingkit/inverse_vector.h>
#include <routingkit/permutation.h>
#include <routingkit/sort.h>
#include <routingkit/timer.h>
#include <routingkit/vector_io.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "verify.h"

using namespace RoutingKit;
using namespace std;

int main(int argc, char* argv[]) {
    try {
        ContractionHierarchy ch;

        cerr << "Loading ... " << flush;
        if (argc != 2) {
            cerr << "Usage : \n" << argv[0] << " ch" << endl;
            return 1;
        } else {
            ch = ContractionHierarchy::load_file(argv[1]);
        }
        cerr << "done" << endl;

        cout << "node_count : " << ch.rank.size() << endl;
        cout << "forward_ch_arc_count : " << ch.forward.head.size() << endl;
        cout << "backward_ch_arc_count : " << ch.backward.head.size() << endl;
    } catch (exception& err) {
        cerr << "Stopped on exception : " << err.what() << endl;
    }
}

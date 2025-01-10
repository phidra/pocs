#include <routingkit/contraction_hierarchy.h>

#include <iostream>

#include "ch_analysis.h"
#include "mygraph.h"

// NOTE : this example is derived from :
//      https://github.com/phidra/RoutingKit/blob/master/README.md

using namespace std;

int main(int argc, char* argv[]) {
    // STEP 1 = build original_graphe :
    auto original_graph = build_original_graph();
    string node_names = "ABCDEFGHIJK";
    assert(node_names.size() == original_graph.num_nodes());
    auto nameof = [&node_names](unsigned node) { return node_names[node]; };
    cout << "node A has id '" << A << "' and name : " << nameof(A) << endl;
    cout << "node C has id '" << C << "' and name : " << nameof(C) << endl;
    cout << "node J has id '" << J << "' and name : " << nameof(J) << endl;
    cout << "node K has id '" << K << "' and name : " << nameof(K) << endl;

    // STEP 2 = contraction + ordering :
    auto ch = RoutingKit::ContractionHierarchy::build(
        original_graph.num_nodes(), original_graph.tails, original_graph.heads, original_graph.weights);

    // STEP 3 = analysis of built CH (this displays lots of interesting infos) :
    analyze_ch(original_graph, ch, nameof);

    // STEP 4 = shortest path from A to H (which should be ABEGH, of cost 5+3+5+3 = 16) :
    RoutingKit::ContractionHierarchyQuery ch_query(ch);
    ch_query.reset().add_source(A).add_target(H).run();
    auto total_cost = ch_query.get_distance();
    auto computed_path_ids = ch_query.get_node_path();
    string path;
    for (auto node : computed_path_ids) {
        path += nameof(node);
    }
    cout << "To get from " << nameof(A) << " to " << nameof(H) << " the cost is of : " << total_cost << endl;
    cout << path << endl;
    assert(path == "ABEGH");
    assert(total_cost == 16);
    cout << "All is OK" << endl;
    cout << endl;
}

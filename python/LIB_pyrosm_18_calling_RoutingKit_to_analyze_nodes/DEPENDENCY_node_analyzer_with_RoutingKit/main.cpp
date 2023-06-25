#include <routingkit/contraction_hierarchy.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "unpack.h"
#include "argparsing.h"

using namespace std;


string id_to_osm_node_url(long node_id) {
    return string("https://www.openstreetmap.org/node/") + to_string(node_id);
}


void usage(string prog_name) {
    cerr << "USAGE:" << endl;
    cerr << "\t" << prog_name << " <TAILS>  <HEADS>  <WEIGHTS>  <OSMIDS>  <NODE-TO-ANALYZE>" << endl;
    cerr << endl;
    cerr << "Where  :" << endl;
    cerr << "\t TAILS / HEADS / WEIGHTS describe edges (as text files containing a single unsigned integer on each line)" << endl;
    cerr << "\t OSMIDS describes NODES (as text file containing a single osmid integer on each line)" << endl;
    cerr << endl;
    cerr << "This program writes its computations on stdout :" << endl;
    cerr << "\t LINE 1 = number of nodes" << endl;
    cerr << "\t LINE 2 = rank of each node (ordered by node id), as a space-separated list of ranks" << endl;
    cerr << "\t LINE 3 = analyzed node info, as space-separated fields :  <rkindex rank osmid osmurl>" << endl;
    cerr << "\t LINE 4 = analysed node's ORIGINAL successor nodes (same field as above, each group being separated by '|')" << endl;
    cerr << "\t LINE 5 = analysed node's CONTRACTED successor nodes (there are additional fields containing the nodes of the unpacked shortcut, as rkindex)" << endl;
    cerr << endl;
    cerr << "Equivalent information are displayed on stderr for humans" << endl;
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

string node_info(unsigned node_rank, RoutingKit::ContractionHierarchy const& ch, vector<long> const& osmids) {
    // on a single line, display space-separated :
    //    - rkindex
    //    - rank
    //    - osmid
    //    - osm url
    auto rkindex = ch.order[node_rank];
    auto osmid = osmids[rkindex];
    ostringstream oss;
    oss << rkindex << " " << ch.rank[rkindex] << " " << osmid << " " << id_to_osm_node_url(osmid);
    return oss.str();
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
    auto node_to_analyze_str = argv[5];

    cerr << "TAILS FILE      = " << tails_file << endl;
    cerr << "HEADS FILE      = " << heads_file << endl;
    cerr << "WEIGHTS FILE    = " << weights_file << endl;
    cerr << "OSMIDS FILE     = " << osmids_file << endl;
    cerr << "NODE TO ANALYZE = " << node_to_analyze_str << endl;

    auto tails = get_vector_u(tails_file, prog_name);
    auto heads = get_vector_u(heads_file, prog_name);
    auto weights = get_vector_u(weights_file, prog_name);
    auto osmids = get_vector_l(osmids_file, prog_name);
    auto node_to_analyze = get_int(node_to_analyze_str, prog_name);
    assert(tails.size() == heads.size());
    assert(tails.size() == weights.size());

    auto NUM_EDGES = tails.size();
    auto NUM_NODES = osmids.size();
    cerr << "NUM NODES    = " << NUM_NODES << endl;
    cerr << "NUM EDGES    = " << NUM_EDGES << endl;


    cerr << "Building CH" << endl;
    auto ch = build_ch(tails, heads, weights, NUM_NODES);

    // stdout line 1 = number of nodes :
    cout << NUM_NODES << endl;
    // stdout line 2 = ranks of the nodes :
    ostringstream oss_rank;
    for (unsigned node_rkindex = 0; node_rkindex < NUM_NODES; ++node_rkindex) {
        oss_rank << ch.rank[node_rkindex] << " ";
    }
    auto oss_rank_str = oss_rank.str();
    cout << oss_rank_str.substr(0, oss_rank_str.size() - 1) << endl;  // remove final " "


    unsigned NODE = node_to_analyze;  // this is the rkindex of the node
    long OSMID = osmids[NODE];
    string OSMURL = id_to_osm_node_url(OSMID);
    unsigned RANK = ch.rank[NODE];
    cerr << "The ANALYZED NODE is the one with rank : " << RANK << " (which has rkindex " << NODE << ") : " << OSMURL << endl;
    cerr << endl;
    cerr << "Displaying its ORIGINAL out-edges :" << endl;
    cout << node_info(RANK, ch, osmids) << endl;
    // stdout line 2 = neighbours of the node on the original graph :
    for (size_t edge = 0; edge < NUM_EDGES; ++edge) {
        auto tail = tails[edge];

        if (tail == NODE) {
            auto head_rkindex = heads[edge];
            auto head = ch.rank[head_rkindex];  // rank
            auto head_osmid = osmids[head_rkindex];
            auto head_osmurl = id_to_osm_node_url(head_osmid);
            auto weight = weights[edge];
            cerr << "\t" << RANK << ">" << head << " (weight=" << weight << "), with URL : " << head_osmurl << endl;
            cout << node_info(head, ch, osmids) << "|";  // there will be a final "|", caller will have to handle it
        }
    }
    cout << endl;

    // NOTE : dans les différentes ch.forward.XXX, les nodes sont stockés par leur rank
    cerr << endl;
    cerr << "Displaying its CONTRACTED out-edges :" << endl;
    for(unsigned arc = ch.forward.first_out[RANK]; arc < ch.forward.first_out[RANK+1]; ++arc) {
        unsigned head = ch.forward.head[arc];  // rank
        auto head_rkindex = ch.order[head];
        auto head_osmid = osmids[head_rkindex];
        auto head_osmurl = id_to_osm_node_url(head_osmid);
        unsigned weight = ch.forward.weight[arc];
        cerr << "\t [" << arc << "] " << RANK << ">" << head << " (weight=" << weight << "), with URL : " << head_osmurl << endl;
        cerr << "\t\t ======== This shortcut goes through the following nodes ========" << endl;

        cout << node_info(head, ch, osmids);

        vector<unsigned> nodes_of_this_shortcut = get_forward_arc_nodes(ch, arc);
        for (auto shortcut_node_rkindex: nodes_of_this_shortcut) {
            auto shortcut_node_osmid = osmids[shortcut_node_rkindex];
            auto shortcut_node_osmurl = id_to_osm_node_url(shortcut_node_osmid);
            cerr << "\t\t " << shortcut_node_rkindex << " : " << shortcut_node_osmurl << endl;
            cout << " " << shortcut_node_rkindex;
        }

        cout << "|";  // there will be a final "|", caller will have to handle it
    }
    cout << endl;
}

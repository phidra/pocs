#ifndef ROUTING_KIT_NESTED_DISSECTION_H
#define ROUTING_KIT_NESTED_DISSECTION_H

#include <routingkit/bit_vector.h>
#include <routingkit/constants.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace RoutingKit {

struct GraphFragment {
    std::vector<unsigned> global_node_id;
    std::vector<unsigned> first_out;
    std::vector<unsigned> tail;
    std::vector<unsigned> head;
    std::vector<unsigned> back_arc;

    unsigned node_count() const { return global_node_id.size(); }

    unsigned arc_count() const { return tail.size(); }
};

GraphFragment make_graph_fragment(unsigned node_count,
                                  std::vector<unsigned> const& tail,
                                  std::vector<unsigned> const& head);

std::vector<GraphFragment> decompose_graph_fragment_into_connected_components(GraphFragment fragment);

struct CutSide {
    unsigned node_on_side_count;
    unsigned cut_size;
    BitVector is_node_on_side;
};

void pick_smaller_side(CutSide& cut);

class BlockingFlow {
   private:
    GraphFragment const* fragment;
    BitVector is_source;
    BitVector is_target;

    unsigned flow_intensity;

    BitVector is_arc_saturated;
    BitVector is_arc_blocked;

    bool is_finished_flag;

   public:
    BlockingFlow() {}
    BlockingFlow(GraphFragment const&, BitVector is_source, BitVector is_target);

    CutSide get_source_cut();
    CutSide get_target_cut();
    CutSide get_balanced_cut();

    void advance();

    unsigned get_current_flow_intensity() const { return flow_intensity; }

    bool is_finished() const { return is_finished_flag; }
};

CutSide inertial_flow(
    GraphFragment const& fragment,
    unsigned min_balance,
    std::vector<float> const& latitude,
    std::vector<float> const& longitude,
    std::function<void(std::string const&)> const& log_message = [](std::string const&) {});

CutSide inertial_flow(
    GraphFragment const& fragment,
    std::vector<float> const& latitude,
    std::vector<float> const& longitude,
    std::function<void(std::string const&)> const& log_message = [](std::string const&) {});

BitVector derive_separator_from_cut(GraphFragment const& fragment, BitVector const& cut);

struct SeparatorDecomposition {
    struct Node {
        unsigned left_child;
        unsigned right_sibling;
        unsigned first_separator_vertex;
        unsigned last_separator_vertex;
    };

    std::vector<Node> tree;
    std::vector<unsigned> order;
};

SeparatorDecomposition compute_separator_decomposition(
    GraphFragment fragment,
    std::function<BitVector(GraphFragment const&)> const& compute_separator,
    std::function<void(std::string const&)> const& log_message = [](std::string const&) {});

std::vector<unsigned> compute_nested_node_dissection_order(
    GraphFragment fragment,
    std::function<BitVector(GraphFragment const&)> const& compute_separator,
    std::function<void(std::string const&)> const& log_message = [](std::string const&) {});

std::vector<unsigned> compute_nested_node_dissection_order_using_inertial_flow(
    unsigned node_count,
    std::vector<unsigned> const& tail,
    std::vector<unsigned> const& head,
    std::vector<float> const& latitude,
    std::vector<float> const& longitude,
    std::function<void(std::string const&)> const& log_message = [](std::string const&) {});

}  // namespace RoutingKit

#endif

#pragma once

#include "mygraph.h"
#include <routingkit/contraction_hierarchy.h>

enum Direction {
    FORWARD,
    BACKWARD
};

std::tuple<unsigned, unsigned, unsigned, bool> get_edge_info(unsigned xy, RoutingKit::ContractionHierarchy const& ch, Direction dir) {
    auto& graph = (dir == FORWARD ? ch.forward : ch.backward);
    auto xy_head_rank = graph.head[xy];
    auto xy_head_node = ch.order[xy_head_rank];
    auto weight = graph.weight[xy];
    bool is_original = graph.is_shortcut_an_original_arc.is_set(xy);
    return {xy_head_rank, xy_head_node, weight, is_original};
}

template <typename TNameOf>
void _out_edges_of(unsigned x_rank, RoutingKit::ContractionHierarchy const & ch, TNameOf const & nameof, Direction dir) {
    unsigned x_node = ch.order[x_rank];
    auto& graph = (dir == FORWARD ? ch.forward : ch.backward);
    for (unsigned xy = graph.first_out[x_rank]; xy < graph.first_out[x_rank+1]; ++xy) {
        auto xy_head_rank = graph.head[xy];
        auto xy_head_node = ch.order[xy_head_rank];
        bool is_original = graph.is_shortcut_an_original_arc.is_set(xy);

        auto [edge_head_rank, edge_head_node, edge_weight, is_edge_original] = get_edge_info(xy, ch, dir);
        std::string dir_string = (dir == FORWARD ? "FORWARD  " : "BACKWARD ");
        std::string original_string = (is_edge_original ? "(ORIGINAL) " : "(SHORTCUT) ");
        std::cout << "\t[id=" << xy << "]  " << dir_string << original_string << nameof(x_node) << nameof(edge_head_node) << " has weight " << edge_weight << std::endl;
        if (! is_edge_original) {
            // je me concentre sur les ids, car je ne m'en sors pas encore pour les interpréter :
            unsigned first_edge_id = graph.shortcut_first_arc[xy];
            unsigned second_edge_id = graph.shortcut_second_arc[xy];
            std::cout << "\t\t FIRST  edge of the shortcut has id " << first_edge_id << std::endl;
            std::cout << "\t\t SECOND edge of the shortcut has id " << second_edge_id<< std::endl;

            std::cout << "\t\tJe m'intéresse au PREMIER edge de ce shortcut, qui a pour id : " << first_edge_id << std::endl;
            std::cout << "\t\t\tCe first edge a pour poids        : " << ch.forward.weight[first_edge_id] << std::endl;
            std::cout << "\t\t\tCe first edge a pour head         : " << ch.forward.head[first_edge_id] << std::endl;
            std::cout << "\t\t\tCe first edge est-il original     ? " << std::boolalpha << ch.forward.is_shortcut_an_original_arc.is_set(first_edge_id) << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_first_arc  F? " << ch.forward.shortcut_first_arc[first_edge_id] << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_second_arc F? " << ch.forward.shortcut_second_arc[first_edge_id] << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_first_arc  B? " << ch.backward.shortcut_first_arc[first_edge_id] << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_second_arc B? " << ch.backward.shortcut_second_arc[first_edge_id] << std::endl;
            std::cout << "\t\t\tMIDDLE NODE PUTATIF ??????????????? " << nameof(ch.backward.shortcut_second_arc[first_edge_id]) << std::endl;

            std::cout << "\t\tJe m'intéresse au SECOND edge de ce shortcut, qui a pour id : " << second_edge_id << std::endl;
            std::cout << "\t\t\tCe second edge a pour poids       : " << ch.forward.weight[second_edge_id] << std::endl;
            std::cout << "\t\t\tCe second edge a pour head        : " << ch.forward.head[second_edge_id] << std::endl;
            std::cout << "\t\t\tCe second edge est-il original    ? " << std::boolalpha << ch.forward.is_shortcut_an_original_arc.is_set(second_edge_id) << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_first_arc  F? " << ch.forward.shortcut_first_arc[second_edge_id] << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_second_arc F? " << ch.forward.shortcut_second_arc[second_edge_id] << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_first_arc  B? " << ch.backward.shortcut_first_arc[second_edge_id] << std::endl;
            std::cout << "\t\t\tQuel est son shortcut_second_arc B? " << ch.backward.shortcut_second_arc[second_edge_id] << std::endl;
            std::cout << "\t\t\tMIDDLE NODE PUTATIF ??????????????? " << nameof(ch.backward.shortcut_second_arc[second_edge_id]) << std::endl;
            /* // ici, comme chacun des deux arcs sont des arcs ORIGINAUX, je peux connaître le middle node grâce à ça : */
            /* unsigned middle_node_rank = graph.shortcut_second_arc[second_edge_id]; */
            /* std::cout << "TRAITEMENT PARTICULIER : middle_node_rank = " << middle_node_rank << std::endl; */
            /* std::cout << "TRAITEMENT PARTICULIER : middle_node name = " << nameof(ch.order[middle_node_rank]) << std::endl; */
            std::cout << std::endl;

            auto [first_edge_head_rank, first_edge_head_node, first_edge_weight, is_first_edge_original] = get_edge_info(first_edge_id, ch, BACKWARD);
            auto [second_edge_head_rank, second_edge_head_node, second_edge_weight, is_second_edge_original] = get_edge_info(second_edge_id, ch, FORWARD);
            std::cout << "\t\t FIRST  edge of the shortcut = " << nameof(x_node) << nameof(first_edge_head_node) << "  (weight=" << first_edge_weight << ", " << (is_first_edge_original ? "ORIGINAL)" : "SHORTCUT)") << std::endl;
            std::cout << "\t\t SECOND edge of the shortcut = " << nameof(second_edge_head_node) << nameof(xy_head_node) << "  (weight=" << second_edge_weight << ", " << (is_second_edge_original ? "ORIGINAL)" : "SHORTCUT)") << std::endl;
        }
    }

}

template <typename TNameOf>
void display_1_generalities(RoutingKit::ContractionHierarchy const& ch, TNameOf const& nameof) {
    // displaying node id :
    for (unsigned node = 0; node != ch.rank.size(); ++ node) {
        std::cout << "node " << node << " has name " << nameof(node) << std::endl;
    }
    std::cout << std::endl;

    // displaying nodes in order of their rank :
    for (unsigned rank = 0; rank != ch.order.size(); ++ rank) {
        std::cout << "node " << nameof(ch.order[rank]) << "(" << ch.order[rank] << ") has rank " << rank << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ch.node_count() is " << ch.node_count() << std::endl;
    std::cout << "ch.rank.size()  is " << ch.rank.size() << std::endl;
    std::cout << "ch.order.size() is " << ch.order.size() << std::endl;
    std::cout << std::endl;

    std::cout << "Sizes of forward/backward :" << std::endl;
    std::cout << "ch.forward.first_out.size() is " << ch.forward.first_out.size() << std::endl;
    std::cout << "ch.forward.head.size() is      " << ch.forward.head.size() << std::endl;
    std::cout << "ch.forward.weight.size() is    " << ch.forward.weight.size() << std::endl;
    std::cout << "ch.forward.shortcut_first_arc.size() is          " << ch.forward.shortcut_first_arc.size() << std::endl;
    std::cout << "ch.forward.shortcut_second_arc.size() is         " << ch.forward.shortcut_second_arc.size() << std::endl;
    std::cout << "ch.forward.is_shortcut_an_original_arc.size() is " << ch.forward.is_shortcut_an_original_arc.size() << std::endl;
    std::cout << std::endl;
    std::cout << "ch.backward.first_out.size() is " << ch.backward.first_out.size() << std::endl;
    std::cout << "ch.backward.head.size() is      " << ch.backward.head.size() << std::endl;
    std::cout << "ch.backward.weight.size() is    " << ch.backward.weight.size() << std::endl;
    std::cout << "ch.backward.shortcut_first_arc.size() is          " << ch.backward.shortcut_first_arc.size() << std::endl;
    std::cout << "ch.backward.shortcut_second_arc.size() is         " << ch.backward.shortcut_second_arc.size() << std::endl;
    std::cout << "ch.backward.is_shortcut_an_original_arc.size() is " << ch.backward.is_shortcut_an_original_arc.size() << std::endl;
    std::cout << std::endl;

}

template <typename TNameOf>
void analyze_graph(RoutingKit::ContractionHierarchy const& ch, Direction dir, TNameOf const& nameof) {
    RoutingKit::ContractionHierarchy::Side const& graph = (dir == FORWARD ? ch.forward : ch.backward);
    for (unsigned rank = 0; rank < ch.node_count(); ++rank) {
        std::cout << "out-edges of " << nameof(ch.order[rank]) << std::endl;
        _out_edges_of(rank, ch, nameof, dir);
    }
}

template <typename TNameOf>
void display_2_forward_backward_graphs(RoutingKit::ContractionHierarchy const& ch, TNameOf const& nameof) {
    // displaying forward/backward graph :
    // IMPORTANT : in ch.forward / ch.backward, nodes seem to be stored BY THEIR RANK
    // It is not initially constructed that way by build_ch_and_order, but is later modified, probably by one of :
    //      optimize_order_for_cache
    //      make_internal_nodes_and_rank_coincide
    //      sort_ch_arcs_and_build_first_out_arrays

    // forward graph :
    std::cout << std::endl;
    std::cout << "=== FORWARD GRAPH (nodes are iterated over by their rank) :" << std::endl;
    analyze_graph(ch, FORWARD, nameof);

    /* // backward graph : */
    /* std::cout << std::endl; */
    /* std::cout << "=== BACKWARD GRAPH (nodes are iterated over by their rank) :" << std::endl; */
    /* analyze_graph(ch, BACKWARD, nameof); */

    display_original_graph();
}

template <typename TNameOf>
void analyze_ch(RoutingKit::ContractionHierarchy const& ch, TNameOf const& nameof) {
    display_1_generalities(ch, nameof);
    display_2_forward_backward_graphs(ch, nameof);
}

#pragma once

#include <iostream>
#include <vector>
#include <cassert>
#include <tuple>

#include <routingkit/contraction_hierarchy.h>

// easy way to set variables to nodes, but we could also `unsigned A = 0` :
enum _ { A = 0, B, C, D, E, F, G, H };

void _add_edge(unsigned tail_node,
               unsigned head_node,
               unsigned edge_weight,
               std::vector<unsigned>& tails,
               std::vector<unsigned>& heads,
               std::vector<unsigned>& weights) {
    assert(tails.size() == heads.size());
    assert(tails.size() == weights.size());
    tails.push_back(tail_node);
    heads.push_back(head_node);
    weights.push_back(edge_weight);
}

void display_original_graph() {
    std::cout << R"DELIM(
               10
           ----------
          /          \
         /            \
        /              \
       /   5        8   \
      A--------B---------C
      |        |         |
      |        |         |
      |1       |3        |5
      |        |         |
      |        |         |
      D--------E         F
           9   |         |
               |         |
               |5        |2
               |         |
               |         |
               G---------H
                    3
    )DELIM" << std::endl;
}

std::tuple<size_t, std::vector<unsigned>, std::vector<unsigned>, std::vector<unsigned>> build_graph() {
    std::cout << "Working on this custom graph : " << std::endl;
    display_original_graph();

    std::vector<unsigned> tails;
    std::vector<unsigned> heads;
    std::vector<unsigned> weights;
    _add_edge(A, B, 5, tails, heads, weights);
    _add_edge(A, C, 10, tails, heads, weights);
    _add_edge(A, D, 1, tails, heads, weights);
    _add_edge(B, C, 8, tails, heads, weights);
    _add_edge(B, E, 3, tails, heads, weights);
    _add_edge(C, F, 5, tails, heads, weights);
    _add_edge(D, E, 9, tails, heads, weights);
    _add_edge(E, G, 5, tails, heads, weights);
    _add_edge(F, H, 2, tails, heads, weights);
    _add_edge(G, H, 3, tails, heads, weights);

    // adding opposite edges (to make graphe bidirectional) :
    _add_edge(B, A, 5, tails, heads, weights);
    _add_edge(C, A, 10, tails, heads, weights);
    _add_edge(D, A, 1, tails, heads, weights);
    _add_edge(C, B, 8, tails, heads, weights);
    _add_edge(E, B, 3, tails, heads, weights);
    _add_edge(F, C, 5, tails, heads, weights);
    _add_edge(E, D, 9, tails, heads, weights);
    _add_edge(G, E, 5, tails, heads, weights);
    _add_edge(H, F, 2, tails, heads, weights);
    _add_edge(H, G, 3, tails, heads, weights);

    constexpr const size_t num_edges = 10;
    constexpr const size_t num_nodes = 8;
    assert(heads.size() == 2 * num_edges);
    assert(tails.size() == 2 * num_edges);
    assert(weights.size() == 2 * num_edges);
    return {num_nodes, tails, heads, weights};
}

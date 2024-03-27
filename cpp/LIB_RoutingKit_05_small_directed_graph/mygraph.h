#pragma once

#include <routingkit/contraction_hierarchy.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

// easy way to set variables to nodes, but we could also `unsigned A = 0` :
enum _ {
    A = 0,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
};

struct OriginalGraph {
    inline size_t num_nodes() const { return _num_nodes; }
    inline size_t num_edges() const { return heads.size(); }
    inline void check_consistency() const { assert(heads.size() == tails.size() && heads.size() == weights.size()); }

    inline void add_edge(unsigned tail_node, unsigned head_node, unsigned edge_weight) {
        tails.push_back(tail_node);
        heads.push_back(head_node);
        weights.push_back(edge_weight);
    }

    inline std::string edgeinfo(size_t edge_index) const {
        std::ostringstream oss;
        unsigned tail = tails[edge_index];
        unsigned head = heads[edge_index];
        unsigned weight = weights[edge_index];
        oss << node_names[tail] << node_names[head] << " (weight=" << weight << ")";
        return oss.str();
    };

    size_t _num_nodes;
    std::vector<unsigned> tails;
    std::vector<unsigned> heads;
    std::vector<unsigned> weights;

    const std::string node_names = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

void display_original_graph() {
    std::cout << R"DELIM(
               10
           ----->----
          /          \
         /            \
        /              \
       /   5        8   \     1
      A---->---B----<----C---->----I
      |        |         |         |
      |        |         |         |
      ^ 1      V 3       ^ 5       v 2
      |        |         |         |
      |        |         |    1    |
      D----<---E         F----<----J
           9   |         |         |
               |         |         |
               V 5       ^ 2       v 2
               |         |         |
               |         |    2    |
               G---->----H----<----K
                    3
    )DELIM" << std::endl;
}

OriginalGraph build_original_graph() {
    std::cout << "Working on this custom graph : " << std::endl;
    display_original_graph();

    OriginalGraph graph;

    // edges are defined from top of the ascii representation to bottom :
    graph.add_edge(A, C, 10);

    graph.add_edge(A, B, 5);
    graph.add_edge(C, B, 8);
    graph.add_edge(C, I, 1);

    graph.add_edge(D, A, 1);
    graph.add_edge(B, E, 3);
    graph.add_edge(F, C, 5);
    graph.add_edge(I, J, 2);

    graph.add_edge(E, D, 9);
    graph.add_edge(J, F, 1);

    graph.add_edge(E, G, 5);
    graph.add_edge(H, F, 2);
    graph.add_edge(J, K, 2);

    graph.add_edge(G, H, 3);
    graph.add_edge(K, H, 2);

    constexpr const size_t NUM_EDGES = 15;
    constexpr const size_t NUM_NODES = 11;

    graph._num_nodes = NUM_NODES;
    graph.check_consistency();
    return graph;
}

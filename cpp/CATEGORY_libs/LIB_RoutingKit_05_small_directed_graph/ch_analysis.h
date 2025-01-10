#pragma once

#include <routingkit/contraction_hierarchy.h>

#include "mygraph.h"

enum Direction { FORWARD, BACKWARD };

struct ContractedEdge {
    unsigned head_rank;
    unsigned head_id;
    unsigned weight;
    unsigned first_half_edge_id;
    unsigned second_half_edge_id;
    bool is_original;

    static inline ContractedEdge create(RoutingKit::ContractionHierarchy const& ch,
                                        RoutingKit::ContractionHierarchy::Side const& graph,
                                        unsigned edge_id) {
        ContractedEdge edge;
        edge.head_rank = graph.head[edge_id];
        edge.head_id = ch.order[edge.head_rank];
        edge.weight = graph.weight[edge_id];
        edge.is_original = graph.is_shortcut_an_original_arc.is_set(edge_id);
        edge.first_half_edge_id = graph.shortcut_first_arc[edge_id];
        edge.second_half_edge_id = graph.shortcut_second_arc[edge_id];
        return edge;
    }
};

std::tuple<unsigned, unsigned, bool> get_edge_info(unsigned xy,
                                                   RoutingKit::ContractionHierarchy const& ch,
                                                   Direction dir) {
    auto& graph = (dir == FORWARD ? ch.forward : ch.backward);
    auto xy_head_rank = graph.head[xy];
    auto xy_head_node = ch.order[xy_head_rank];
    auto weight = graph.weight[xy];
    bool is_original = graph.is_shortcut_an_original_arc.is_set(xy);
    return {xy_head_node, weight, is_original};
}

template <typename TNameOf>
void _out_edges_of(OriginalGraph const& original_graph,
                   unsigned x_rank,
                   RoutingKit::ContractionHierarchy const& ch,
                   TNameOf const& nameof,
                   Direction dir) {
    unsigned x_node = ch.order[x_rank];
    auto& graph = (dir == FORWARD ? ch.forward : ch.backward);

    auto nameofrank = [&ch, &nameof](unsigned rank) { return nameof(ch.order[rank]); };
    for (unsigned xy = graph.first_out[x_rank]; xy < graph.first_out[x_rank + 1]; ++xy) {
        // head of the edge :
        auto xy_head_rank = graph.head[xy];
        auto xy_head_node = ch.order[xy_head_rank];
        bool is_original = graph.is_shortcut_an_original_arc.is_set(xy);

        ContractedEdge edge = ContractedEdge::create(ch, graph, xy);
        std::string dir_string = (dir == FORWARD ? "FORWARD  " : "BACKWARD ");
        std::string original_string = (edge.is_original ? "(ORIGINAL) " : "(SHORTCUT) ");
        std::cout << "\t[id=" << xy << "]  " << dir_string << original_string << nameof(x_node) << nameof(edge.head_id)
                  << " has weight " << edge.weight << std::endl;
        if (!edge.is_original) {
            std::cout << "\t\t FIRST  edge of the shortcut has id " << edge.first_half_edge_id << std::endl;
            std::cout << "\t\t SECOND edge of the shortcut has id " << edge.second_half_edge_id << std::endl;

            // NOTE 1 = premier point important à comprendre : first_half_edge_id et second_half_edge_id (qui sont des
            // index d'edges) ne réfèrent pas à la même structure !
            //      - shortcut_first_arc est un edge-index dans ch.backward
            //      - shortcut_second_arc est un edge-index dans ch.forward
            // Ceci découle du fait que les rank d'un shortcut AB (ayant pour middle-node X) sont nécessairement X < A <
            // B :
            //      - AB est un donc un edge BACKWARD
            //      - XB est un donc un edge FORWARD
            // Du coup, quand on regarde les infos de chaque demi-edge, il faut regarder dans ch.forward OU ch.backward.

            // Le FIRST demi-edge est à rechercher dans le graphe BACKWARD :
            ContractedEdge half_edge_1 = ContractedEdge::create(ch, ch.backward, edge.first_half_edge_id);

            // Le SECOND demi-edge est à rechercher dans le graphe FORWARD :
            ContractedEdge half_edge_2 = ContractedEdge::create(ch, ch.forward, edge.second_half_edge_id);

            // NOTE 2 : si un (demi-)edge est un edge ORIGINAL, alors shortcut_first_arc et shortcut_second_arc se
            // réfèrent au graphe ORIGINAL :
            //      - shortcut_first_arc est alors l'index de l'edge original
            //      - shortcut_second_arc est alors l'id du tail de l'edge original
            // cf. ce commentaire :
            //      https://github.com/phidra/RoutingKit/blob/a0776b234ac6e86d4255952ef60a6a9bf8d88f02/include/routingkit/contraction_hierarchy.h#L60
            // Le graphe original dont il est question est celui passé à ContractionHierarchy::build (le triplet
            // head+tail/weight). Et comme il n'est pas stocké dans la structure ContractionHierarchy, interpréter
            // shortcut_first/second_arc pour un edge original NÉCESSITE le graphe original (c'est pour ça qu'il est
            // passé en argument de la présente fonction)

            // Le code ci-dessous montre que shortcut_first_arc est l'edge original, et shortcut_second_arc est le tail
            // de celui-ci : Je ne peux illustrer ceci qu'aver le PREMIER demi-edge, car pour le second, il y a (je
            // pense) un bug dans RoutingKit...
            if (half_edge_1.is_original) {
                unsigned edge_tail_in_the_original_graph = ch.backward.shortcut_second_arc[edge.first_half_edge_id];

                unsigned edge_index_in_the_original_graph = ch.backward.shortcut_first_arc[edge.first_half_edge_id];
                // ici, comme on travaille sur le premier demi-edge, c'est un edge backward.
                // Le tail d'un edge backward est en fait un head dans le graphe original (donc on utilise
                // original_graph.heads) :
                unsigned edge_tail_retrieved = original_graph.heads[edge_index_in_the_original_graph];
                assert(edge_tail_retrieved == edge_tail_in_the_original_graph);
            }

            std::cout << "\t\tJe m'intéresse au PREMIER demi-edge de ce shortcut (c'est donc un edge du graphe "
                         "BACKWARD), qui a pour backward-id : "
                      << edge.first_half_edge_id << std::endl;
            std::cout << "\t\t\tCe demi-edge a pour poids               : " << half_edge_1.weight << std::endl;
            std::cout << "\t\t\tCe demi-edge a pour head                : " << half_edge_1.head_rank << " = "
                      << nameof(half_edge_1.head_id) << std::endl;
            std::cout << "\t\t\tCe demi-edge est-il original            ? " << std::boolalpha << half_edge_1.is_original
                      << std::endl;
            // NOTE : comme mon graphe est simpliste, un demi-edge sera TOUJOURS original, les shortcut_first/second_arc
            // des demi-arcs représentent donc un edge réel, ce que j'exploite (en affichant l'edge/tail original) :
            std::cout << "\t\t\tCe demi-edge a pour shortcut_first_arc  : " << half_edge_1.first_half_edge_id << " = "
                      << original_graph.edgeinfo(half_edge_1.first_half_edge_id) << std::endl;
            std::cout << "\t\t\tCe demi-edge a pour shortcut_second_arc : " << half_edge_1.second_half_edge_id << " = "
                      << nameof(half_edge_1.second_half_edge_id) << std::endl;

            std::cout << "\t\tJe m'intéresse au DEUXIEME demi-edge de ce shortcut (c'est donc un edge du graphe "
                         "FORWARD), qui a pour forward-id : "
                      << edge.second_half_edge_id << std::endl;
            std::cout << "\t\t\tCe demi-edge a pour poids               : " << half_edge_2.weight << std::endl;
            std::cout << "\t\t\tCe demi-edge a pour head                : " << half_edge_2.head_rank << " = "
                      << nameof(half_edge_2.head_id) << std::endl;
            std::cout << "\t\t\tCe demi-edge est-il original            ? " << std::boolalpha << half_edge_2.is_original
                      << std::endl;
            // NOTE : comme mon graphe est simpliste, un demi-edge sera TOUJOURS original, les shortcut_first/second_arc
            // des demi-arcs représentent donc un edge réel, ce que j'exploite (en affichant l'edge/tail original) :
            std::cout << "\t\t\tCe demi-edge a pour shortcut_first_arc  : " << half_edge_2.first_half_edge_id << " = "
                      << original_graph.edgeinfo(half_edge_2.first_half_edge_id) << std::endl;
            std::cout << "\t\t\tCe demi-edge a pour shortcut_second_arc : " << half_edge_2.second_half_edge_id << " = "
                      << nameof(half_edge_2.second_half_edge_id) << std::endl;
            std::cout << "\t\t\t                                          ^  CECI N'EST PAS LE TAIL DU DEMI-EDGE (mais "
                         "son head), CAR IL Y A UN BUG DANS RoutingKit..."
                      << std::endl;

            // NOTE 3 : concernant le middle-node d'un shortcut (ce qui était mon besoin original) :
            // je pense qu'one ne peut pas (facilement) récupérer le middle nodes des shortcuts
            // le seul cas où on peut le faire (ce qui est mon cas ici vu la simplicité de mon graphe) est celui où l'un
            // des deux demi-edges est un edge ORIGINAL en effet, dans ce cas, j'ai accès (via
            // shortcut_first/second_arc) à toutes les infos de l'edge original, y compris son tail (en pratique, il y a
            // selon moi un bug dans routing-kit, et je n'ai directemetn accès au tail du node que pour le SECOND
            // demi-edge)
        }
    }
}

template <typename TNameOf>
void display_1_generalities(RoutingKit::ContractionHierarchy const& ch, TNameOf const& nameof) {
    // displaying node id :
    for (unsigned node = 0; node != ch.rank.size(); ++node) {
        std::cout << "node " << node << " has name " << nameof(node) << std::endl;
    }
    std::cout << std::endl;

    // displaying nodes in order of their rank :
    for (unsigned rank = 0; rank != ch.order.size(); ++rank) {
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
    std::cout << "ch.forward.shortcut_first_arc.size() is          " << ch.forward.shortcut_first_arc.size()
              << std::endl;
    std::cout << "ch.forward.shortcut_second_arc.size() is         " << ch.forward.shortcut_second_arc.size()
              << std::endl;
    std::cout << "ch.forward.is_shortcut_an_original_arc.size() is " << ch.forward.is_shortcut_an_original_arc.size()
              << std::endl;
    std::cout << std::endl;
    std::cout << "ch.backward.first_out.size() is " << ch.backward.first_out.size() << std::endl;
    std::cout << "ch.backward.head.size() is      " << ch.backward.head.size() << std::endl;
    std::cout << "ch.backward.weight.size() is    " << ch.backward.weight.size() << std::endl;
    std::cout << "ch.backward.shortcut_first_arc.size() is          " << ch.backward.shortcut_first_arc.size()
              << std::endl;
    std::cout << "ch.backward.shortcut_second_arc.size() is         " << ch.backward.shortcut_second_arc.size()
              << std::endl;
    std::cout << "ch.backward.is_shortcut_an_original_arc.size() is " << ch.backward.is_shortcut_an_original_arc.size()
              << std::endl;
    std::cout << std::endl;
}

template <typename TNameOf>
void analyze_graph(OriginalGraph const& original_graph,
                   RoutingKit::ContractionHierarchy const& ch,
                   Direction dir,
                   TNameOf const& nameof) {
    RoutingKit::ContractionHierarchy::Side const& graph = (dir == FORWARD ? ch.forward : ch.backward);
    for (unsigned rank = 0; rank < ch.node_count(); ++rank) {
        std::cout << "out-edges of " << nameof(ch.order[rank]) << std::endl;
        _out_edges_of(original_graph, rank, ch, nameof, dir);
    }
}

template <typename TNameOf>
void display_2_forward_graph(OriginalGraph const& original_graph,
                             RoutingKit::ContractionHierarchy const& ch,
                             TNameOf const& nameof) {
    // IMPORTANT : in ch.forward / ch.backward, nodes seem to be stored BY THEIR RANK

    // forward graph :
    std::cout << std::endl;
    std::cout << "=== FORWARD GRAPH (nodes are iterated over by their rank) :" << std::endl;
    analyze_graph(original_graph, ch, FORWARD, nameof);
}

template <typename TNameOf>
void analyze_ch(OriginalGraph const& original_graph,
                RoutingKit::ContractionHierarchy const& ch,
                TNameOf const& nameof) {
    display_1_generalities(ch, nameof);
    display_2_forward_graph(original_graph, ch, nameof);
    display_original_graph();
}

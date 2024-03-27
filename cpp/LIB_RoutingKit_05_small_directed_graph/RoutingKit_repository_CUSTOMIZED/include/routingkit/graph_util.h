#ifndef ROUTING_KIT_GRAPH_UTIL_H
#define ROUTING_KIT_GRAPH_UTIL_H

#include <vector>

namespace RoutingKit {

std::vector<unsigned> convert_node_path_to_arc_path(std::vector<unsigned> const& first_out,
                                                    std::vector<unsigned> const& head,
                                                    std::vector<unsigned> path);
std::vector<unsigned> convert_arc_path_to_node_path(unsigned source,
                                                    std::vector<unsigned> const& head,
                                                    std::vector<unsigned> path);

unsigned find_arc(std::vector<unsigned> const& first_out, std::vector<unsigned> const& head, unsigned x, unsigned y);
unsigned find_arc_or_return_invalid(std::vector<unsigned> const& first_out,
                                    std::vector<unsigned> const& head,
                                    unsigned x,
                                    unsigned y);

unsigned find_arc_given_sorted_head(std::vector<unsigned> const& first_out,
                                    std::vector<unsigned> const& head,
                                    unsigned x,
                                    unsigned y);
unsigned find_arc_or_return_invalid_given_sorted_head(std::vector<unsigned> const& first_out,
                                                      std::vector<unsigned> const& head,
                                                      unsigned x,
                                                      unsigned y);

std::vector<unsigned> compute_inverse_sort_permutation_first_by_left_then_by_right(unsigned a_count,
                                                                                   std::vector<unsigned> const& a,
                                                                                   unsigned b_count,
                                                                                   std::vector<unsigned> const& b);

std::vector<unsigned> compute_sort_permutation_first_by_left_then_by_right(unsigned a_count,
                                                                           std::vector<unsigned> const& a,
                                                                           unsigned b_count,
                                                                           std::vector<unsigned> const& b);

std::vector<unsigned> compute_inverse_sort_permutation_first_by_left_then_by_right_and_apply_sort_to_left(
    unsigned a_count,
    std::vector<unsigned>& a,
    unsigned b_count,
    std::vector<unsigned> const& b);

std::vector<unsigned> compute_inverse_sort_permutation_first_by_tail_then_by_head(unsigned node_count,
                                                                                  std::vector<unsigned> const& tail,
                                                                                  std::vector<unsigned> const& head);

std::vector<unsigned> compute_sort_permutation_first_by_tail_then_by_head(unsigned node_count,
                                                                          std::vector<unsigned> const& tail,
                                                                          std::vector<unsigned> const& head);

std::vector<unsigned> compute_inverse_sort_permutation_first_by_tail_then_by_head_and_apply_sort_to_tail(
    unsigned node_count,
    std::vector<unsigned>& tail,
    std::vector<unsigned> const& head);

std::vector<unsigned> compute_sort_permutation_first_by_tail_then_by_head_and_apply_sort_to_tail(
    unsigned node_count,
    std::vector<unsigned>& tail,
    std::vector<unsigned> const& head);

}  // namespace RoutingKit

#endif

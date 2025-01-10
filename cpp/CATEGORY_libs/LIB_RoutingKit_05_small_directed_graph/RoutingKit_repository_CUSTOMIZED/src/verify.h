#ifndef VERIFY_H
#define VERIFY_H

#include <vector>

namespace RoutingKit {

void check_if_graph_is_valid(std::vector<unsigned> const& first_out, std::vector<unsigned> const& head);

void check_if_td_graph_is_valid(unsigned period,
                                std::vector<unsigned> const& first_out,
                                std::vector<unsigned> const& head,
                                std::vector<unsigned> const& first_ipp_of_arc,
                                std::vector<unsigned> const& ipp_departure_time,
                                std::vector<unsigned> const& ipp_travel_time);

void check_if_arc_ipp_are_valid(unsigned period,
                                std::vector<unsigned> const& first_ipp_of_arc,
                                std::vector<unsigned> const& ipp_departure_time,
                                std::vector<unsigned> const& ipp_travel_time);

void check_if_sst_queries_are_valid(unsigned period,
                                    unsigned node_count,
                                    std::vector<unsigned> const& source,
                                    std::vector<unsigned> const& source_time,
                                    std::vector<unsigned> const& target,
                                    std::vector<unsigned> const& rank);

}  // namespace RoutingKit

#endif

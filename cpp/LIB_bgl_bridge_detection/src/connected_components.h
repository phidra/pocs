#pragma once

#include <vector>

#include "graphtypes.h"

// note : "components" are designed by "vertices", to reduce confusion with boost functions

// connected components :
std::vector<std::vector<VertexDescriptor>> compute_connected_vertices(Graph const& graph);
void display_connected_vertices(Graph const& graph,
                                std::vector<std::vector<VertexDescriptor>> const& connected_vertices,
                                const int threshold = 0);
void dump_connected_vertices_hull(Graph const& graph,
                                  std::vector<std::vector<VertexDescriptor>> const& connected_vertices,
                                  std::string const& output_dir);

// biconnected components :
struct BiconnResults {
    std::map<EdgeDescriptor, int> edge2biconn;
    std::map<int, std::set<EdgeDescriptor>> biconn2edges;
    int nb_of_biconn_comps;
    std::vector<VertexDescriptor> art_points_descriptors;
    std::vector<Point> art_points;
    std::vector<MultiPoint> biconn_components_points;
};
BiconnResults compute_biconnected_vertices(Graph const& graph);
void dump_biconnected_vertices_hull(Graph const& graph, BiconnResults const& biconns, std::string const& output_dir);

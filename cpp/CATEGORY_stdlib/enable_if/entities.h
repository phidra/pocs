#pragma once

#include <vector>
#include <string>

// A bunch of classes that the POC uses.
// Some of thoses have a 'toString' method, some don't.

struct Person {
    std::string first_name;
    std::string last_name;
    int age;
    Person(std::string first_name_, std::string last_name_, int age_)
        : first_name(first_name_), last_name(last_name_), age(age_) {}
    std::string toString() const { return std::string("PERSON> ") + first_name + std::string(" ") + last_name; }
};

struct Building {
    std::string address;
    int height;
    Building(std::string address_, int height_) : address(address_), height(height_) {}
    std::string toString() const { return std::string("BUILDING located at '") + address + "'"; }
};

struct Graph {
    using Node = size_t;
    using Weight = float;
    int num_nodes;
    int num_edges;
    std::vector<size_t> adjarray_src_node;  // points to first-node in edges
    std::vector<std::pair<Node, Weight> > adjarray_edges;
    Graph(int num_nodes_, int num_edges_) : num_nodes(num_nodes_), num_edges(num_edges_) {
        adjarray_src_node.reserve(num_nodes);
        adjarray_edges.reserve(num_edges + 1);  // one more for sentinel
    }
};

#pragma once

#include <routingkit/contraction_hierarchy.h>

// this is pasted from :
// https://github.com/phidra/RoutingKit/blob/a0776b234ac6e86d4255952ef60a6a9bf8d88f02/src/contraction_hierarchy.cpp#L1712

// OnNewInputArc has the signature
//   on_new_arc(unsigned xy, unsigned y)
// It is called for every arc xy of the path. y is the head of xy.
// The source node of the path must be obtained by some other mean

template <class OnNewInputArc>
void unpack_forward_arc(RoutingKit::ContractionHierarchy const& ch,
                        unsigned arc,
                        OnNewInputArc const& on_new_input_arc);

template <class OnNewInputArc>
void unpack_backward_arc(RoutingKit::ContractionHierarchy const& ch,
                         unsigned arc,
                         OnNewInputArc const& on_new_input_arc);

template <class OnNewInputArc>
void unpack_forward_arc(RoutingKit::ContractionHierarchy const& ch,
                        unsigned arc,
                        OnNewInputArc const& on_new_input_arc) {
    if (ch.forward.is_shortcut_an_original_arc.is_set(arc)) {
        on_new_input_arc(ch.forward.shortcut_first_arc[arc], ch.forward.shortcut_second_arc[arc]);
    } else {
        assert(ch.forward.shortcut_first_arc[arc] < ch.backward.head.size());
        assert(ch.forward.shortcut_second_arc[arc] < ch.forward.head.size());
        unpack_backward_arc(ch, ch.forward.shortcut_first_arc[arc], on_new_input_arc);
        unpack_forward_arc(ch, ch.forward.shortcut_second_arc[arc], on_new_input_arc);
    }
}

template <class OnNewInputArc>
void unpack_backward_arc(RoutingKit::ContractionHierarchy const& ch,
                         unsigned arc,
                         OnNewInputArc const& on_new_input_arc) {
    if (ch.backward.is_shortcut_an_original_arc.is_set(arc)) {
        on_new_input_arc(ch.backward.shortcut_first_arc[arc], ch.backward.shortcut_second_arc[arc]);
    } else {
        assert(ch.backward.shortcut_first_arc[arc] < ch.backward.head.size());
        assert(ch.backward.shortcut_second_arc[arc] < ch.forward.head.size());
        unpack_backward_arc(ch, ch.backward.shortcut_first_arc[arc], on_new_input_arc);
        unpack_forward_arc(ch, ch.backward.shortcut_second_arc[arc], on_new_input_arc);
    }
}

std::vector<unsigned> get_forward_arc_nodes(RoutingKit::ContractionHierarchy const& ch, size_t arc) {
    std::vector<unsigned> path;
    auto callback = [&path](unsigned xy, unsigned y) { path.push_back(y); };
    unpack_forward_arc(ch, static_cast<unsigned>(arc), callback);
    // WARNING 1 : the returned nodes are the node's RKINDEX (and not the node ranks, as in the rest of CH code) !
    // WARNING 2 : from a shortcut AB, this returns all nodes from A to B... EXCEPT A !
    // NOTE : this functions works fine even if the arc is not a shortcut but a real arc.
    return path;
}

#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

boost et Breadth-First Search (aka BFS) :
	https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/breadth_first_search.html
	https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/graph_theory_review.html#sec:bfs-algorithm
	https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/bibliography.html#moore59
	vertex discover = la première fois qu'on rencontre un vertex
	vertex finish = le moment où on a fini d'explorer tous les voisins d'un vertex
	color map = pour indiquer les vertex qu'on a déjà rencontrés (NdM : inutile pour un tree, mais indispensable sinon)
		White vertices are undiscovered
		while gray vertices are discovered but have undiscovered adjacent vertices
		Black vertices are discovered and are adjacent to only other black or gray vertices.
	queue = la structure de données stockant les vertex discover
	description de l'algorithme :
		The algorithm proceeds by removing a vertex u from the queue and examining each out-edge (u,v).
		If an adjacent vertex v is not already discovered, it is colored gray and placed in the queue.
		After all of the out-edges are examined, vertex u is colored black and the process is repeated.
	par défaut, l'implémentation de boost ne stocke pas les parents, ou la distance (mais peut le faire facilement)

EOF


pocs="${1:-"$(ls poc*.cpp)"}"


echo "Building and running pocs : ${pocs}"

echo ""
for poc in ${pocs}
do
    bin="bin_${poc%%.cpp}"
    echo "===== building $poc"
    rm -f "$bin"
    clang++ -std=c++11 -Wall "$poc" -o "$bin"
    chmod +x "$bin"
done

for poc in ${pocs}
do
    bin="bin_${poc%%.cpp}"
    echo ""
    echo "===== running ${bin}"
    ./"$bin"
done

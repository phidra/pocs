#!/usr/bin/env python3

import pygraphviz as pgv


def draw_png(mygraph, path):
    # OUTPUT = drawing graphviz :
    mygraph.layout()
    mygraph.draw(path)


def draw_dot(mygraph, path):
    # OUTPUT = string graphviz :
    mygraph_as_dot_string = mygraph.string()
    with open(path, "w") as f:
        f.write(mygraph_as_dot_string)


def STEP1_from_dotfile():
    # INPUT = chargement du graphe depuis un fichier dot :
    g = pgv.AGraph("pouet.dot")
    draw_png(g, "/tmp/NOGIT_pouet.png")
    draw_dot(g, "/tmp/NOGIT_pouet.dot")


def STEP2_from_dict_of_dict():
    # INPUT = dict of dict :
    d = {"1": {"2": None}, "2": {"1": None, "3": None}, "3": {"2": None}}
    g = pgv.AGraph(d)
    draw_png(g, "/tmp/NOGIT_othergraph.png")


def STEP3_manual_build():
    g = pgv.AGraph()
    g.add_node("A")
    g.add_node("B")
    g.add_node("C")
    g.add_edge("A", "B")
    g.add_edge("B", "C")
    g.add_edge("C", "A")

    nodelist = ["D", "E", "F"]
    g.add_nodes_from(nodelist)

    edgelist = [("D", "E"), ("E", "F"), ("F", "D")]
    g.add_edges_from(edgelist)

    g.add_edge("A", "D")

    draw_png(g, "/tmp/NOGIT_third.png")


STEP1_from_dotfile()
STEP2_from_dict_of_dict()
STEP3_manual_build()

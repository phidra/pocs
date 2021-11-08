#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

# possibly :
#   sudo apt install texlive-latex-base
#   sudo apt install texlive-latex-recommended texlive-latex-extra texlive-pictures
#   sudo apt install pandoc

pandoc \
    --toc \
    --strip-comments \
    --slide-level=2 \
    -fmarkdown-implicit_figures \
    -t beamer \
    demo.md \
    -o NOGIT_demo.pdf

cp NOGIT_demo.pdf /tmp  # delete this at the end, easy retrieval

# other conversion are available, among which :
# pandoc -t revealjs demo.md -o NOGIT_demo_reveal.html
# pandoc -t slidy demo.md -o NOGIT_demo_slidy.html


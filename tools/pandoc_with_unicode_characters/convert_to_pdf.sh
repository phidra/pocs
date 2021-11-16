#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


# possibly :
#   sudo apt install texlive-latex-base
#   sudo apt install texlive-latex-recommended texlive-latex-extra texlive-pictures
#   sudo apt install pandoc

# when compared to the previous POCS, this is new, and needed to use xelatex :
#   sudo apt install texlive-full

pandoc \
    --toc \
    --strip-comments \
    --slide-level=2 \
    --pdf-engine=xelatex \
    -t beamer \
    demo.md \
    -o NOGIT_slides.pdf

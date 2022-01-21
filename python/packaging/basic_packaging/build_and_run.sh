#!/bin/bash

set -o errexit
set -o nounset


echo """
This POC packages a module, installs and use it in a virtualenv.
Package parameters are defined in setup.py.


"""

# cleaning :
rm -rf dist/
rm -rf ./*.egg-info/
rm -rf NOGIT_myvenv/

# building :
python3 setup.py sdist

# checking content of python package (veryimportant should be in it) :
tar -tvzf dist/*.tar.gz


# using module in a virtualenv :
deactivate || true
rm -rf NOGIT_myvenv
/usr/bin/env python3 -m venv  NOGIT_myvenv
NOGIT_myvenv/bin/pip install wheel
NOGIT_myvenv/bin/pip install dist/basic1-0.0.1.tar.gz

NOGIT_myvenv/bin/python -c "from usefulmodule.veryimportant import what_is_the_answer ; what_is_the_answer()"

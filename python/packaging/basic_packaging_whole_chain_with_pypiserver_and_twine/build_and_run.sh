#!/bin/bash

set -o errexit
set -o nounset


echo """
This POC packages a module, uploads it to a pypi-server with twine, and uses it in a virtualenv.

It requires pypiserver and twine :
    pip install pypiserver
    pip install twine


"""

# cleaning :
rm -rf dist/
rm -rf ./*.egg-info/
rm -rf NOGIT_myvenv/

# building :
python3 setup.py sdist

# checking content of python package (veryimportant should be in it) :
tar -tvzf dist/*.tar.gz

# creating venv :
rm -rf NOGIT_myvenv
/usr/bin/env python3 -m venv  NOGIT_myvenv
NOGIT_myvenv/bin/pip install wheel

NOGIT_myvenv/bin/pip install pypiserver
NOGIT_myvenv/bin/pip install twine

# Running a pypi server in background :
pypi_packages_dir=$(mktemp -d --suffix=_POC_on_python_packaging)
pypi_server_port=12345
echo ""
echo "Running a PYPI server in background, it will serve : ${pypi_packages_dir}"
echo "If necessary, command to kill it :"
echo ""
echo "    pkill -n pypi-server"
echo ""
NOGIT_myvenv/bin/pypi-server -P . -a . -p ${pypi_server_port} "${pypi_packages_dir}" &

# uploading package to pypi-server :
sleep 1
echo ""
echo "Uploading to pypi-server with twine :"
TWINE_PASSWORD="dummy" TWINE_USERNAME="dummy" NOGIT_myvenv/bin/twine upload --repository-url http://localhost:${pypi_server_port} dist/*  --verbose
echo "Done"
echo ""

# using module in a virtualenv :
NOGIT_myvenv/bin/pip install -i http://localhost:${pypi_server_port} basic1
NOGIT_myvenv/bin/python -c "from usefulmodule.veryimportant import what_is_the_answer ; what_is_the_answer()"

# cleaning :
echo ""
echo "Killing pypi-server instance :"
sleep 1
pkill -f -n pypi-server
echo "Done"
echo ""
echo ""
# pgrep -f is mandatory, see : https://unix.stackexchange.com/questions/267007/pgrep-full-match-not-work-only-part-why/267021#267021
echo "If there is a pypi-server process below, you'll have to kill it manually :"
pgrep -f pypi-server

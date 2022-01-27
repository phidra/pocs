#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


echo """
This POC shows a basic C++ extension module for python, that uses dicts.
"""



# cleaning :
rm -rf "${this_script_parent}/build/"

# execution :
echo ""
echo "=== Without building the module, execution fails :"
set +o errexit
python3 main.py
echo ""
echo "EXPECTED RESULT = ModuleNotFoundError: No module named 'pouet'"
echo ""
set -o errexit

# building :
python3 "${this_script_parent}/setup.py" build_ext

# putting my 'pouet' module in PYTHONPATH :
# (not sure that this is the proper way to construct the build dir, but it works on ubuntu 20.04) :
platform="$(python3 -c "import sys ; print(sys.platform)")"
py_version="$(python3 -c "import sys ; print(f'{sys.version_info.major}.{sys.version_info.minor}')")"
arch="$(uname -p)"
build_dir="${this_script_parent}/build/lib.${platform}-${arch}-${py_version}"  # build/lib.linux-x86_64-3.8
export PYTHONPATH="${build_dir}"

# execution :
echo ""
echo "=== after building the module, execution is ok :"
python3 main.py
echo "EXPECTED RESULT = a bunch of 'OK' :-)"

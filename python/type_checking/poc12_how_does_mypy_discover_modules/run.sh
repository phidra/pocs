#!/bin/bash

# set -o errexit  # this line is commented because we expect on of the mypy runs to fail
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"


cat << EOF

The intent of the POC is to show how mypy discover modules to analyze :

What does the POC do :
    - it defines 1 package containing 2 modules (one of them has a type error, the other is ok)
    - it defines 1 main that only uses the ok-module
    - it runs mypy using two different invocations :
        + mypy main.py
        + mypy .

What I learn from this POC :
    - mypy discovers modules to analyze through import
    - thus 'mypy main.py' does not detect the bug, because the error-module is never imported
    - the special case 'mypy .' does detect the bug, because it recursively search for all python file in dir
    - see : https://mypy.readthedocs.io/en/stable/running_mypy.html#mapping-file-paths-to-modules

EXPECTED :
    - 'mypy main.py' is GREEN (it does not detect any error, as it does not analyze the (non-imported) error-module)
    - 'mypy .' is RED (it does detect an error in the error-module)

EOF

# execution is alright :
echo ""
echo "RUNNING THE MAIN :"
python3 "${this_script_parent}/main.py"

# running mypy solely on 'main.py' is ALSO green, as the error-module is never imported :
echo ""
echo "RUNNING MYPY 1 = SOLELY ON THE MAIN :"
mypy "${this_script_parent}/main.py"

# but running mypy on the whole folder is red, as mypy then recursively run on all modules, including the error one :
echo ""
echo "RUNNING MYPY 2 = ON THE WHOLE FOLDER :"
mypy "${this_script_parent}"

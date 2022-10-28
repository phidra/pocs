#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

Various problematic situations when RAII does not work :
- uncaught exception
- std::exit (and its cousins terminate and abort)
- ...

In a witness case, normally returning from main properly calls destructor.

In all those cases, we are LOCKING a resource, and UNLOCKING it in a RAII struct.
In the problematic cases, the UNLOCKING is not done !

CONCLUSION : RAII does not ALWAYS free resources, we must be a bit careful (use a catchall, do not call exit/terminate/abort, etc.)

EOF

g++ main0_witness.cpp -o NOGIT_bin0_witness && ./NOGIT_bin0_witness

set +o errexit

g++ main1_uncaught_exceptions.cpp -o NOGIT_bin1_uncaught_exceptions && ./NOGIT_bin1_uncaught_exceptions
g++ main2_std_exit.cpp -o NOGIT_bin2_std_exit && ./NOGIT_bin2_std_exit

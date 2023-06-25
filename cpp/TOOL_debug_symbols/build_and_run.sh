#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

echo """
This POC shows how the "-g" option helps the debugger.

When running debugger on a binary with debug-infos, the gdb output is richer.

https://en.wikipedia.org/wiki/Debugging_data_format
High-level debuggers need information about variables, types, constants, subroutines and so on, so they can translate between machine-level storage and source language constructs.

https://en.wikipedia.org/wiki/Debug_symbol
A debug symbol is a special kind of symbol that attaches additional information to the symbol table of an object file, such as a shared library or an executable.
This information allows a symbolic debugger to gain access to information from the source code of the binary, such as the names of identifiers, including variables and routines.

https://en.wikipedia.org/wiki/DWARF
DWARF is a widely used, standardized debugging data format.

"""


# building release :
rm -rf cmake_build_dir_release
echo ""
echo "===== CONFIGURE (cmake) + BUILD (make) RELEASE BEGIN"
cmake -DCMAKE_BUILD_TYPE=Release -Bcmake_build_dir_release -H.
make -C cmake_build_dir_release
echo "===== CONFIGURE (cmake) + BUILD (make) RELEASE END"
echo ""

# building debug :
rm -rf cmake_build_dir_debug
echo ""
echo "===== CONFIGURE (cmake) + BUILD (make) DEBUG BEGIN"
cmake -DCMAKE_BUILD_TYPE=Debug -Bcmake_build_dir_debug -H.
make -C cmake_build_dir_debug
echo "===== CONFIGURE (cmake) + BUILD (make) DEBUG END"
echo ""


echo ""
echo "===== RUN DEBUG (bin) BEGIN"
set +o errexit  # the binary returns the sum of its args as returncode (thus non-zero)
cmake_build_dir_debug/bin 10 8
RETCODE=$?
echo "EXPECTED RETURN CODE = 18"
echo "OBTAINED RETURN CODE = $RETCODE"
echo "===== RUN DEBUG (bin) END"
echo ""


# hinting the use of gdb :
echo ""
echo "You can now run :"
echo "gdb cmake_build_dir/bin    (OR ON bin_debug)"
echo "    > break parse"
echo "    > run 10 8"
echo "    > backtrace"
echo ""
echo "EXAMPLE :"
echo 'gdb cmake_build_dir_release/bin -ex "break parse" -ex "run 10 8" -ex "backtrace"'
echo 'gdb cmake_build_dir_debug/bin   -ex "break parse" -ex "run 10 8" -ex "backtrace"'
echo ""


# running gdb in release :
gdb cmake_build_dir_debug/bin -ex "break parse" -ex "run 10 8" -ex "backtrace"



# SAMPLE :

# gdb cmake_build_dir/bin
#
# (gdb) break parse
# Breakpoint 1 at 0x1149
#
# (gdb) run 10 8
# Starting program: /path/to/pocs/cpp/debug_symbols/cmake_build_dir/bin 10 8
# 
# Breakpoint 1, 0x0000555555555149 in parse(char*) ()
#
# (gdb) backtrace
# #0  0x0000555555555149 in parse(char*) ()
# #1  0x000055555555518e in main ()

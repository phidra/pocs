#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
BUILD_DIR="${this_script_parent}/NOGIT_build"
mkdir -p "${BUILD_DIR}"
g++ -std=c++17 main.cpp -O0 -g -Wall -Werror -o "${this_script_parent}/NOGIT_bin"

# regular run :
"${this_script_parent}/NOGIT_bin"

# run with GDB, with GDB modifying the displayed string :
# NOTE : the difference between this POC and the one with int, is that we use string.assign(...)
gdb "${this_script_parent}/NOGIT_bin" < "${this_script_parent}/commands.gdb"

# Expected gdb output :
# Reading symbols from /media/DATA/git_projects/pocs/cpp/TOOL_gdb/01_modify_variable/NOGIT_bin...
# (gdb) Breakpoint 1 at 0x2449: file main.cpp, line 4.
# (gdb) Starting program: /media/DATA/git_projects/pocs/cpp/TOOL_gdb/01_modify_variable/NOGIT_bin

# This POC shows how to modify a string with gdb.
# The C++ code is simple : display a string.
# The POC's interest is in gdb commands : calling 'to_display.assign' to modify the displayed string
# BEFORE call #0

# Breakpoint 1, myfunction (to_display=<error: Cannot access memory at address 0x8d4c5741fa1e0ff3>) at main.cpp:4
# 4       void myfunction(std::string const& to_display) {
# (gdb) (gdb) (gdb) Continuing.
# Let me introduce you... pouet
# AFTER  call #0

# BEFORE call #1

# Breakpoint 1, myfunction (to_display=<error: Cannot access memory at address 0x8d4c5741fa1e0ff3>) at main.cpp:4
# 4       void myfunction(std::string const& to_display) {
# (gdb) (gdb) (gdb) (gdb) 5           std::cout << "Let me introduce you... " << to_display << std::endl;
# (gdb) $1 = '=' <repeats 20 times>, "THIS VALUE IS SET BY GDB, YAY !", '=' <repeats 24 times>
# (gdb) Continuing.
# Let me introduce you... ====================THIS VALUE IS SET BY GDB, YAY !========================
# AFTER  call #1

# BEFORE call #2

# Breakpoint 1, myfunction (to_display=<error: Cannot access memory at address 0x8d4c5741fa1e0ff3>) at main.cpp:4
# 4       void myfunction(std::string const& to_display) {
# (gdb) (gdb) (gdb) Continuing.
# Let me introduce you... pouet
# AFTER  call #2

# [Inferior 1 (process 105066) exited normally]

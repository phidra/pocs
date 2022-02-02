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

# run with GDB, with GDB modifying the displayed int :
gdb "${this_script_parent}/NOGIT_bin" < "${this_script_parent}/commands.gdb"

# EXPECTED OUTPUT :
# Reading symbols from /media/DATA/git_projects/pocs/cpp/TOOL_gdb/01_modify_int/NOGIT_bin...
# (gdb) Breakpoint 1 at 0x11c9: file main.cpp, line 3.
# (gdb) Starting program: /media/DATA/git_projects/pocs/cpp/TOOL_gdb/01_modify_int/NOGIT_bin

# This POC shows how to modify an int with gdb.
# The C++ code is simple : display an int.
# The POC's interest is in gdb commands : setting value of 'age' to modify its display.
# BEFORE call #0

# Breakpoint 1, myfunction (age=32767) at main.cpp:3
# 3       void myfunction(int age) {
# (gdb) (gdb) "


# ==== breaking on the first call = leave the function unchanged :
# "(gdb) Continuing.
# Captain's age is 42
# AFTER  call #0

# BEFORE call #1

# Breakpoint 1, myfunction (age=32767) at main.cpp:3
# 3       void myfunction(int age) {
# (gdb) (gdb) "


# ==== breaking on the second call = assign another value to the int :
# "(gdb) (gdb) 4      std::cout << "Captain's age is " << age << std::endl;
# (gdb) type = int
# (gdb) (gdb) Continuing.
# Captain's age is 999999
# AFTER  call #1

# BEFORE call #2

# Breakpoint 1, myfunction (age=32767) at main.cpp:3
# 3       void myfunction(int age) {
# (gdb) (gdb) "


# ==== breaking on the third call = leave the function unchanged again :
# "(gdb) Continuing.
# Captain's age is 42
# AFTER  call #2

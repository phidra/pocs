#!/bin/bash

set -o errexit
set -o nounset

echo """
This POC shows that usage of strace to see syscalls when they happen.

EXPECTED :
    1. the (verbose) beginning of strace, ending with nanosleep
    2. a small wait
    3. a syscall to getppid in strace
"""

echo ""
echo "Building C program :"
gcc main.c -o bin.GITIGNORE

echo ""
echo "====== strace on bin :"
strace ./bin.GITIGNORE
PPID=$?

echo ""
echo "Parent pid was $PPID"
sleep 1



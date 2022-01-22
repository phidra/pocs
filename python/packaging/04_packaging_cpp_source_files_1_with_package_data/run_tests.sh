#!/bin/bash

g++ test/basic_test.cpp -I src src/lib.cpp -o test/testbin && test/testbin

#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF
This POCs shows that namespaced type-alias can overwrite a global type-alias.

This runs the "good" situation, where no type-alias overwrite occur.

(see also the corresponding run_KO.sh, when type-alias overwrite occur, and causes segfault, with detailed explanation)


EOF

rm -f NOGIT_bin_OK
mkdir -p _build_OK
g++ -g -c src/main_OK.cpp -o _build_OK/main_OK.o
g++ -g -c src/builder.cpp -o _build_OK/builder.o
g++ -g -c src/pouet.cpp -o _build_OK/pouet.o  # this translation unit is not really necessary
g++  _build_OK/main_OK.o _build_OK/builder.o -o NOGIT_bin_OK

echo ""
echo "Showing that both 'builder.o' and 'main_OK.o' have some object code to destroy the vector :"
echo "The interesting part, is that 'builder' and 'main' both destroy a vector of INTS"
echo "(this is different from the 'OK' situation, where 'main' destroys STRINGS and 'builder' destroys INTS)"
set -o xtrace
nm --demangle _build_OK/builder.o|grep '~vector'

nm --demangle _build_OK/main_OK.o|grep '~vector'
set +o xtrace

# EXPECTED OUTPUT :
# + nm --demangle _build_OK/builder.o
# + grep '~vector'
# 0000000000000000 W std::vector<int, std::allocator<int> >::~vector()
# 0000000000000000 W std::vector<int, std::allocator<int> >::~vector()
# 0000000000000000 n std::vector<int, std::allocator<int> >::~vector()
# + nm --demangle _build_OK/main_OK.o
# + grep '~vector'
# 0000000000000000 W std::vector<int, std::allocator<int> >::~vector()
# 0000000000000000 W std::vector<int, std::allocator<int> >::~vector()
# 0000000000000000 n std::vector<int, std::allocator<int> >::~vector()

echo ""
echo "Running executable, expecting no segfault :"
echo "(no segfault, because both object-code destroy the same objects = ints)"
./NOGIT_bin_OK

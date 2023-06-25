#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF
This POCs shows that namespaced type-alias can overwrite a global type-alias.

Here :
-   ::StopId is an int
- my::StopId is a string
- in main_KO.cpp, my::build is used to create an object with a member (hum) which is a vector<int>
- because of the type alias overwrite, when this object is destroyed at the end of the scope, vector<string>::~vector is called
- this causes segfault because :
    + the vector's destruction calls the destructor of its items
    + as the caller of ~vector is 'main', it is ~vector<string> that is called
    + BUT, it is called on an object built by 'builder'... which is a vector<int> !
    + thus, we try to call the string's destructor on an int -> segfault

Run gdb to confirm that segfault occurs at vector<string>::~vector :

    gdb NOGIT_bin_KO -ex "run"

EXAMPLE OF STACKTRACE (showing that the destructor call is vector<string>::~vector) :
    Program received signal SIGSEGV, Segmentation fault.
    __GI___libc_free (mem=0x400000003) at malloc.c:3102
    3102	malloc.c: Aucun fichier ou dossier de ce type.
    (gdb) bt
    #0  __GI___libc_free (mem=0x400000003) at malloc.c:3102
    #1  0x0000555555555953 in std::_Destroy<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > (__pointer=0x55555556cef0) at /usr/include/c++/9/bits/stl_construct.h:98
    #2  0x00005555555558cc in std::_Destroy_aux<false>::__destroy<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*> (__first=0x55555556cef0, __last=0x55555556cf04) at /usr/include/c++/9/bits/stl_construct.h:108
    #3  0x0000555555555820 in std::_Destroy<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*> (__first=0x55555556cef0, __last=0x55555556cf04) at /usr/include/c++/9/bits/stl_construct.h:137
    #4  0x0000555555555739 in std::_Destroy<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > (__first=0x55555556cef0, __last=0x55555556cf04) at /usr/include/c++/9/bits/stl_construct.h:206
    #5  0x00005555555555a1 in std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >::~vector (this=0x7fffffffd988, __in_chrg=<optimized out>)
        at /usr/include/c++/9/bits/stl_vector.h:677
    #6  0x00005555555554b0 in my::Pouet::~Pouet (this=0x7fffffffd940, __in_chrg=<optimized out>) at src/pouet.h:13
    #7  0x00005555555553b4 in main () at src/main_KO.cpp:23

SOME QUESTIONS :
Q1: in which object-file is the object-code for the vector's destructor defined ?
A1: there are TWO object-code for the vector's destructor : in 'main_KO.o' and in 'builder.o'.
    in the KO situation, both versions are different : 'main' has ~vector<string> whereas 'builder' has ~vector<int>
Q2: why does this code compile ?
A2: there is no reason for the compilation to fail : from each translation-unit point of view (main and builder), all is consistent.
    (main's translation-unit doesn't know that there exist some vector<int> elsewhere, and builder's TU doesn't know that there exist some vector<string>)
Q3: why does this code link ?
A3: this is a bit trickier, my guess is that linker only checks for symbol name, and not type.
    more precisely : the linker doesn't check that the return-type of build() is the same in main.o and builder.o
    and as long as the "build()" symbol is resolved, it doesn't bother the linker that there are TWO possibilities to resolve it

(see also the corresponding run_OK.sh, when no type-alias overwrite occur)


EOF

rm -f NOGIT_bin_KO
mkdir -p _build_KO
g++ -g -c src/main_KO.cpp -o _build_KO/main_KO.o
g++ -g -c src/builder.cpp -o _build_KO/builder.o
g++ -g -c src/pouet.cpp -o _build_KO/pouet.o  # this translation unit is not really necessary
g++  _build_KO/main_KO.o _build_KO/builder.o -o NOGIT_bin_KO

# interestingly, with this object code order, the segfault does not appear :
# probably because the symbol for the vector destructor is then resolved on the builder's vector's destructor
# (which is a vector<int> and not a vector<string>)
# g++ _build_KO/builder.o _build_KO/main_KO.o -o NOGIT_bin_KO

echo ""
echo "Showing that both 'builder.o' and 'main_KO.o' have some object code to destroy the vector :"
echo "The interesting part, is that 'builder' can destroy a vector of INTS, whereas 'main' can destroy a vector of STRINGS"
echo "(this is different from the 'OK' situation, where both 'main' and 'builder' destroy a vector of INTS)"
set -o xtrace
nm --demangle _build_KO/builder.o|grep '~vector'

nm --demangle _build_KO/main_KO.o|grep '~vector'
set +o xtrace

# EXPECTED OUTPUT :
# + nm --demangle _build_KO/builder.o
# + grep '~vector'
# 0000000000000000 W std::vector<int, std::allocator<int> >::~vector()
# 0000000000000000 W std::vector<int, std::allocator<int> >::~vector()
# 0000000000000000 n std::vector<int, std::allocator<int> >::~vector()
# + grep '~vector'
# + nm --demangle _build_KO/main_KO.o
# 0000000000000000 W std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >::~vector()
# 0000000000000000 W std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >::~vector()
# 0000000000000000 n std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >::~vector()


echo ""
echo "As illustrated, both main_KO.o and builder.o have some object code to destroy the vector."
echo "Due to the StopId mismatch, builder's destructor destroys INTS , whereas main's destructor destroys STRINGS"
echo ""

echo ""
echo "Running executable, expecting a segfault :"
echo "(segfault is due to string destructor being applied by 'main' on the ints built by 'builder')"
./NOGIT_bin_KO

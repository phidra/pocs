#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

This POC shows how to set/unset variables in cmake cache.

Context : why is iwyu still run while I removed the option ?! (answer : the option was cached)

To SET a cache variable :
    solution 1 = option -D    (https://cmake.org/cmake/help/latest/manual/cmake.1.html#options)
    solution 2 = command set  (https://cmake.org/cmake/help/latest/command/set.html)
To UNSET a cache variale :
    solution 1 = option -U    (https://cmake.org/cmake/help/latest/manual/cmake.1.html#options)
    solution 2 = command unset  (https://cmake.org/cmake/help/latest/command/unset.html)
    solution 3 = edit CMakeCache.txt

EOF


trap 'error_handler' ERR
error_handler() {
    echo ""
    echo ""
    echo "ERROR : the script didn't exit properly, a cmake output is probably not as expected !!!"
}


# STEP 0 = clean initial state (with empty cache, and no envvar) :
rm -rf _build
mkdir _build
unset SET_CACHED_POUET


# NOTE : all the 'greps' (with the errexit) ensure that the POC "proves" that the result are as expected

# STEP 1 = with an empty cache :
echo ""
echo "========= STEP 1 = Execution with an empty cache :"
echo "          (expected = no 'POUET' in cache)"
cmake -B _build . 2>&1 | grep "POUET is NOT defined"


# STEP 2 = -D option to set cache variable :
echo ""
echo "========= STEP 2 = Execution with -DPOUET=coucou :"
echo "          (expected = 'POUET' in cache with value 'coucou')"
cmake -B _build -DPOUET:STRING=coucou . 2>&1 | grep "POUET is defined"


# STEP 3 = no -D option, but cache variable is still alive :
echo ""
echo "========= STEP 3 = Execution without -D option (but cache still contains the previous POUET)"
echo "          (expected = 'POUET' still in cache, as kept from the previous configure)"
cmake -B _build . 2>&1 | grep "POUET is defined"


# STEP 4 = to remove cache entry (possibility 1), use -U option :
echo ""
echo "========= STEP 4 = Execution with -UPOUET :"
echo "          (expected = no 'POUET' in cache, it was removed by -U option)"
cmake -B _build -UPOUET . 2>&1 | grep "POUET is NOT defined"


# STEP 5 = we can also set a cache variable from CMake script :
# (note : the set of the cached variable is trigged by an envvar 'SET_CACHED_POUET')
echo ""
echo "========= STEP 5 = Putting back a new cache value, then building again (without option) :"
echo "          (expected = 'POUET' in cache, with a different value = youpi)"
SET_CACHED_POUET=1 cmake -B _build . 2>&1 | grep "POUET is defined"


# STEP 6 = to remove cache entry (possibility 2), we use 'unset' :
echo ""
echo "========= STEP 6 = Removing the cached POUET directly from cmake script :"
echo "          (expected = no 'POUET' in cache, it was removed by unset in the cmake script)"
UNSET_CACHED_POUET=1 cmake -B _build . 2>&1 | grep "POUET is NOT defined"


# STEP 7 = this step doesn't prove anything, but we neeed the cached POUET to be set for the next step :
echo ""
echo "========= STEP 7 = (setting again the cached value)"
SET_CACHED_POUET=1 cmake -B _build . 2>&1 | grep "POUET is defined" &> /dev/null


# STEP 8 = to remove cache entry (possibility 3), we directly edit the cache file :
echo ""
echo "========= STEP 8 = Removing the cached POUET by editing the CMakeCache.txt file :"
echo "          (expected = no 'POUET' in cache, it was removed from the cache file)"
# IMPORTANT : we must remove the cache variable AND the comment that precedes it (otherwise cmake parsing fails)
sed --in-place "/POUET/d" _build/CMakeCache.txt
sed --in-place "/Just a dummy value/d" _build/CMakeCache.txt
cmake -B _build . 2>&1 | grep "POUET is NOT defined"

echo ""
echo "The POC ran properly, no error occured"  # trap ensures that this is only displayed if no error occured

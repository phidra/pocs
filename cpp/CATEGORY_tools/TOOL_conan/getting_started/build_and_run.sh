# see: https://docs.conan.io/en/latest/getting_started.html

# without these, link fails :
conan profile new default --detect 
conan profile update settings.compiler.libcxx=libstdc++11 default


rm -rf cmake_build_dir
mkdir cmake_build_dir && cd cmake_build_dir && conan install ..
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .

# md5 computed with our binary:
./bin/md5 

# md5 computed with shell tool :
echo -n -e "abcdefghijklmnopqrstuvwxyz" | md5sum

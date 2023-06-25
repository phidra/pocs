# the lib defines an empty function named "pass"
#
# without LTO, the compilation of main.cpp is unable to see that
# the code in lib.cpp does nothing, and don't optimize it.
#
# with LTO, the optimization is made at link-time, and
# the code is optimized : the function is not called, and 
# the empty loop is cleared.

rm -f lib.o main.o bin
g++ -std=c++11 -O3 lib.cpp -c -o lib.o
g++ -std=c++11 -O3 main.cpp -c -o main.o
g++ -std=c++11 -O3 main.o lib.o -o bin  
echo ""
echo "compilation séparée, sans LTO :"
./bin

rm -f lib.o main.o bin
g++ -std=c++11 -O3 -flto lib.cpp -c -o lib.o
g++ -std=c++11 -O3 -flto main.cpp -c -o main.o
g++ -std=c++11 -O3 -flto main.o lib.o -o bin  
echo ""
echo "compilation séparée, avec LTO :"
./bin

rm -f lib.o main.o bin
g++ -std=c++11 -O3 main.cpp lib.cpp -o bin  
echo ""
echo "compilation en une seule ligne, sans LTO :"
./bin

rm -f lib.o main.o bin
g++ -std=c++11 -O3 -flto main.cpp lib.cpp -o bin  
echo ""
echo "compilation en une seule ligne, avec LTO :"
./bin


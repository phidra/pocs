#include <iostream>

using std::cout;
using std::endl;



int main(int argc, char* argv[]) // argc = ARGument Count  /  argv = ARGument Vector
{
    cout << "Façon n°1 :" << endl;
    for (int i = 0; i < argc; ++i)
    {
        cout << "\t arg #" << i << " = " << argv[i] << endl;
    }
    cout << endl;
    
    // cette façon de faire fonctionne car argv[argc] est garanti d'être un pointeur nul
    cout << "Façon n°2 :" << endl;
    int i = 0;
    char* arg = nullptr;
    while(arg = argv[i])
    {
        cout << "\t arg #" << i << " = " << arg << endl;
        ++i;
    }
    cout << endl;
}

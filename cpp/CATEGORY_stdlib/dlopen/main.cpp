#include <iostream>
#include <dlfcn.h>
#include <string>

using namespace std;

constexpr const char* FUNCTION_NAME = "process";  // don't forget to compile libs with extern C !
constexpr const char* DESCRIPTION = "description";
constexpr const char* NO_DESCRIPTION = "[NO DESCRIPTION]";
using ProcessorFunction = int (*)(int);

int do_the_work(const char* lib)
{
    cout << endl;
    cout << "Loaded lib will be : " << lib << endl;
    auto handle = dlopen(lib, RTLD_NOW);
    const char* lib_open_error = dlerror();
    if (!handle)
    {
        cerr << "ERROR : unable to dlopen '" << lib << "' : " << lib_open_error << endl;
        return 10;
    }

    dlerror();  // clear error code
    ProcessorFunction processor = reinterpret_cast<ProcessorFunction>(dlsym(handle, FUNCTION_NAME));
    const char* function_load_error = dlerror();
    if (function_load_error != NULL)
    {
        cerr << "ERROR : unable to load function name '" << FUNCTION_NAME << "' : " << function_load_error << endl;
        dlclose(handle);
        return 20;
    }


    dlerror();  // clear error code
    const char* const* description = reinterpret_cast<const char* const*>(dlsym(handle, DESCRIPTION));
    if (dlerror() != NULL)
    {
        description = &NO_DESCRIPTION;
    }

    cout << "Lib description  = " << *description << endl;


    int initial_number = 18;
    cout << "initial number   = " << initial_number << endl;
    int processed_number = (*processor)(initial_number);
    cout << "processed number = " << processed_number << endl;

    auto closing_status = dlclose(handle);
    const char* lib_close_error = dlerror();
    if (closing_status != 0)
    {
        cerr << "ERROR : unable to dlclose : " << lib << lib_close_error << endl;
        return 30;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int status = -1;
    do_the_work("/tmp/libmul2.so" );
    do_the_work("/tmp/libmul10.so");
    do_the_work("/tmp/libadd42.so");
    do_the_work("/tmp/libinexisting.so");
    do_the_work("/tmp/libirrelevant.so");
    return 0;
}

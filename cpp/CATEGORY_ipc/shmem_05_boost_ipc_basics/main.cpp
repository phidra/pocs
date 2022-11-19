#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib>  //std::system
#include <sstream>
#include <iostream>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = POC montrant une utilisation simple de boost::interprocess::managed_shared_memory

C'est l'exemple donné par le quick-guide de boost IPC :
    https://www.boost.org/doc/libs/1_80_0/doc/html/interprocess/quick_guide.html


Le process parent créée une managed_shared_memory, alloue 1 kilobyte, et remplit les 5 premiers byte.
Il appelle de façon synchrone alors un process enfant en lui transmettant le handle de sa shared-memory.
Le process enfant affiche le contenu del a mémoire, puis la désalloue, et rend la main au process parent.
Quand il récupère la main, le process parent vérifie que la mémoire a bien été désallouée.

)DELIM";
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    print_poc_description();

    using namespace boost::interprocess;
    if (argc == 1) {  // Parent process
        // Remove shared memory on construction and destruction
        struct shm_remove {
            shm_remove() { shared_memory_object::remove("MySharedMemory"); }
            ~shm_remove() { shared_memory_object::remove("MySharedMemory"); }
        } remover;

        // Create a managed shared memory segment
        managed_shared_memory segment(create_only, "MySharedMemory", 65536);

        // Allocate a portion of the segment (raw memory)
        managed_shared_memory::size_type free_memory = segment.get_free_memory();
        void* shptr = segment.allocate(1024 /*bytes to allocate*/);

        // Check invariant
        if (free_memory <= segment.get_free_memory())
            return 1;

        auto tab = static_cast<char*>(shptr);
        tab[0] = 'L';
        tab[1] = 'U';
        tab[2] = 'K';
        tab[3] = 'E';
        tab[4] = '\0';
        std::cout << "SENDER> " << tab << std::endl;

        // An handle from the base address can identify any byte of the shared
        // memory segment even if it is mapped in different base addresses
        managed_shared_memory::handle_t handle = segment.get_handle_from_address(shptr);
        std::stringstream s;
        s << argv[0] << " " << handle;
        s << std::ends;
        // Launch child process
        if (0 != std::system(s.str().c_str()))
            return 1;
        // Check memory has been freed
        if (free_memory != segment.get_free_memory())
            return 1;
    } else {
        // Open managed segment
        managed_shared_memory segment(open_only, "MySharedMemory");

        // An handle from the base address can identify any byte of the shared
        // memory segment even if it is mapped in different base addresses
        managed_shared_memory::handle_t handle = 0;

        // Obtain handle value
        std::stringstream s;
        s << argv[1];
        s >> handle;

        // Get buffer local address from handle
        void* msg = segment.get_address_from_handle(handle);

        auto tab = static_cast<char*>(msg);
        std::cout << "RECEIVER> " << tab << std::endl;

        // Deallocate previously allocated memory
        segment.deallocate(msg);
    }
    return 0;
}

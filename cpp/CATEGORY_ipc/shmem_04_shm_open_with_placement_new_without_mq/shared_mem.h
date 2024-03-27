#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>

#include "utils.h"

template <class T>
struct SharedMem {
    int file_descriptor;
    T* ptr;
    std::string name;
    bool is_owner;

    SharedMem(std::string name_) : name{name_}, is_owner{false} {
        file_descriptor = shm_open(name.c_str(), O_RDWR | O_CREAT, 0600);
        if (file_descriptor == -1) {
            raise_with_strerrno("Failed to open a shared memory region");
        }

        if (ftruncate(file_descriptor, sizeof(T)) < 0) {
            close(file_descriptor);
            raise_with_strerrno("Failed to set size of a shared memory region");
        };

        void* mmapped = mmap(nullptr, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
        if (!mmapped) {
            close(file_descriptor);
            raise_with_strerrno("Failed to mmap a shared memory region");
        }

        // construct object in mmapped memory :
        ptr = new (mmapped) T{};
    }

    void set_owner() { is_owner = true; }

    ~SharedMem() {
        munmap(ptr, sizeof(T));
        if (close(file_descriptor) == -1) {
            raise_with_strerrno("Failed to close file-descriptor");
        }

        if (is_owner) {
            // explicitly destroying :
            ptr->~T();

            std::cout << "Unlinking shared mem instance " << name << std::endl;
            // Pour cette POC, on ignore les erreurs.
            // Particulièrement, ENOENT (= on essaye d'unlink une shared-mem inexistante) est ignoré
            shm_unlink(name.c_str());
            // À noter qu'on peut shm_unlink plus tôt : dès qu'on a mmappé le segment en mémoire.
            // Si on shm_unlink comme ceci, ça se contente de détruire le nom (et le chemin dans le tmpfs).
            // Mais shm_unlink ne détruit pas la mémoire, juste le nom.
            // cf. https://stackoverflow.com/questions/65860154/behaviour-of-shm-unlink/65860605#65860605
        }
    }

    T& get() const { return *ptr; }
};

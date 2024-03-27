#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>

template <class T>
class SharedMem {
    // NDM : cette structure SharedMem et tout ce qu'elle contient est propre à chaque process.
    //       Chaque process en aura une copie à lui !
    //       Ce qui est partagé entre les process (donc unique), c'est ce vers quoi pointe le T*.
    int file_descriptor;
    T* ptr;

    // NDM : parmi les différentes SharedMem dupliquées entre tous les process, l'une d'elle sera considérée
    //       comme "owner", elle sera en charge de shm_unlink(name) la mémoire partagée.
    //       On la reconnaît car c'est la seule pour laquelle 'name' n'est pas nullptr.
    char const* name;

   public:
    SharedMem(char const* shared_mem_name, bool owner = false) {
        // NDM : comme on passe O_CREAT, peu importe que la shared-memory existe déjà ou pas.
        //       conséquence un peu contre-intuitive = ce n'est pas nécessairement l'owner qui va créer la shared-mem !
        file_descriptor = shm_open(shared_mem_name, O_RDWR | O_CREAT, 0600);
        if (file_descriptor == -1) {
            throw std::runtime_error("Failed to open a shared memory region");
        }

        // NDM : ici, le file-descriptor désigne un "fichier" dans un tmpfs
        //       du coup, ftruncate s'applique à une région de la mémoire.
        if (ftruncate(file_descriptor, sizeof(T)) < 0) {
            close(file_descriptor);
            throw std::runtime_error("Failed to set size of a shared memory region");
        };

        // NDM : c'est ici que le mmapping est fait :
        //    nullptr = on laisse le kernel choisir l'addresse virtuelle du mapping (page-aligned)
        //    sizeof(T) = on ne mappe que ce qui est nécessaire pour accéder à T
        //                et comme le "fichier" (en tmpfs) fait exactement sizeof(T), au final
        //                on mappe l'ensemble du "fichier" (en vrai, la région mémoire)
        //    PROT_READ | PROT_WRITE = on peut lire et écrire sur la mémoire partagée
        //    MAP_SHARED = la mémoire est partagée avec d'autres process
        //    file_descriptor = la mémoire virtuelle est backée par ce "fichier" (mais en vrai, de la RAM)
        ptr = static_cast<T*>(mmap(nullptr, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0));

        // NDM : vérification expérimentale que la mémoire partagée est initialisée à des zéros.
        //       je suis à peu près sûr que c'est ftruncate qui est responsable de ça, vu le man ftruncate :
        //           If the file previously was shorter, it is extended, and the extended part reads as null bytes
        //           ('\0').
        std::cout << "size of the shared-memory = sizeof(T) = " << sizeof(T) << std::endl;
        std::cout << "content of the shared-memory :" << std::endl;
        for (unsigned long offset = 0; offset < sizeof(T); ++offset) {
            char value = *(reinterpret_cast<char*>(ptr + offset));
            std::cout << "\t byte n°" << offset << " = " << std::hex << static_cast<int>(value) << std::endl;
        }
        std::cout << std::endl;

        // QUESTION = il me semblait qu'il fallait être aligné sur la memory-page pour mmap ? Or là on utilise sizeof(T)
        // ?
        //    non, j'ai confondu deux choses : l'addresse du mapping (qui doit effectivement être page-alignée) et sa
        //    taille ici, c'est la taille qui vaut sizeof(T) et l'addresse étant à nullptr, le kernel choisira lui-même
        //    une addresse page-aligned

        if (!ptr) {
            close(file_descriptor);
            throw std::runtime_error("Failed to mmap a shared memory region");
        }

        // NDM : le seul cas où on mémorise le name ouvert avec shm_open, c'est si le process est owner.
        this->name = owner ? shared_mem_name : nullptr;
    }

    ~SharedMem() {
        munmap(ptr, sizeof(T));
        close(file_descriptor);
        if (name) {
            std::cout << "Remove shared mem instance " << name << std::endl;
            shm_unlink(name);
            // À noter qu'on peut shm_unlink plus tôt : dès qu'on a mmappé le segment en mémoire.
            // Si on shm_unlink comme ceci, ça se contente de détruire le nom (et le chemin dans le tmpfs).
            // Mais shm_unlink ne détruit pas la mémoire, juste le nom.
            // cf. https://stackoverflow.com/questions/65860154/behaviour-of-shm-unlink/65860605#65860605
        }
    }

    T& get() const { return *ptr; }
};

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <new>

#include "utils.h"

struct A {
    A(int initial_value) { internal_value = initial_value; }
    void write(int value) { internal_value = value; }
    int read() const { return internal_value; }
    void watch(int value) {
        while (internal_value == value)
            usleep(1000);
    }
    std::atomic<int> internal_value;
};

using namespace std;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = un exemple (pas le plus canonique...) de deux process qui communiquent par mémoire partagée.

Cette POC est la transcription directe de ce post :
    https://lucisqr.substack.com/p/processes-or-threads

La POC est intéressante car elle jette les bases de l'IPC (mmap sur un fichier avec MAP_SHARED, placement-new + atomic
pour synchroniser), mais elle fait un peu n'importe quoi : chaque process écrit dans la shared-mem sans se soucier de ce que l'autre y a écrit.

Ma compréhension résumée de cette POC = partage de mémoire entre process avec mmap d'un fichier sur disque-dur :

- le fichier est sur disque, et on suppose qu'il n'a jamais été lu (donc il n'est pas en RAM)
- mmap par le process A  :
    le mmap dans le process A réserve une plage d'adresses virtuelle du process A, backée par le fichier sur disque
    (pour simplifier, on suppose que cette plage est représentée par une unique memory-page)
    au premier accès en lecture de cette memory-page, on se mange une page-fault
    le fichier est ainsi mis en RAM par le kernel, i.e. il créée une memory-frame MF qui contient le contenu du fichier
    à ce stade, la memory-page virtuelle du process A est backée par la memory-frame MF
    le process A peut donc accéder à la mémoire dans la memory frame
- mmap par le process B :
    mmap dans le process B réserve une plage d'adresses virtuelle de B (pour simplifier : une unique memory-page)
    ici, le fichier est déjà en RAM dans le kernel : la memory-page de B est backée par la memory-frame MF
    c'est bien la MÊME memory-frame MF qui backe à la fois la page de A et la page de B !
- la mémoire est donc effectivement partagée entre les process, CQFD

Petite note complémentaire apportée par la libc :
    https://www.gnu.org/software/libc/manual/2.36/html_node/Memory_002dmapped-I_002fO.html
    MAP_SHARED  Changes made will be shared immediately with other processes mmaping the same file.
                Note that actual writing may take place at any time.


EDIT : notes vrac complémentaires sur ce que je comprends de flock :
    c'est plutôt un truc BSD (la libc ne parle que de fcntl)
    https://github.com/torvalds/linux/blob/8f71a2b3f435f29b787537d1abedaa7d8ebe6647/Documentation/filesystems/locks.rst
        The solution I have chosen, after much experimentation and discussion, is to make flock() and fcntl() locks oblivious to each other. Both can exists, and neither will have any effect on the other.
        ----------------------------------------
        les deux sont incompatibles et indépendants
    NE PAS CONFONDRE :
        la FONCTION flock décrite ci-dessus (pas hyper-claire qui la pourvoit du coup, si c'est pas le kernel linux ni la libc ?)
        la STRUCTURE flock de la libc
    https://www.gnu.org/software/libc/manual/2.36/html_node/File-Locks.html
        TODO = prendre des notes (en gros, le lock est acquis par un process avec fcntl)
    https://www.gnu.org/software/libc/manual/2.36/html_node/Control-Operations.html
        les opérations qu'on peut faire avec fcntl

)DELIM";
    cout << endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // STEP 1 = ouvrir un fichier qui accueillera la MMAP :
    char const* filename = "/tmp/shared.dat";
    int fid = ::open(filename, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    // cf. man open :
    //     O_CREAT If pathname does not exist, create it as a regular file.
    //     S_IRWXU  00700 user (file owner) has read, write, and execute permission
    //     S_IRWXG  00070 group has read, write, and execute permission
    //     O_RDWR.  These request opening the file [...] read/write.

    // STEP 2 = on mmappe le fichier dans l'espace mémoire du processus.
    int mapsize = sysconf(_SC_PAGE_SIZE);
    void* ptr = ::mmap(nullptr, mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, fid, 0);
    // cf. man mmap :
    //     offset must be a multiple of the page size as returned by sysconf(_SC_PAGE_SIZE).
    //     PROT_READ  Pages may be read.
    //     PROT_WRITE Pages may be written.
    //     MAP_SHARED Share this mapping.  Updates to the mapping are visible to other processes mapping the same
    //     region, and (in the case of file-backed mappings) are carried through to the underlying file.  (To precisely
    //     control when updates are carried through to the underlying file requires the use of msync(2).)

    // STEP 3 = on récupère un pointeur sur la structure A (en la créant si elle n'existe pas) :
    // cf. man flock :
    //     Apply or remove an advisory lock on the open file specified by fd.
    //     LOCK_EX  Place an exclusive lock.
    //     LOCK_UN  Remove an existing lock held by this process.
    A* a;
    int value;

    {
        FileLock lock{fid, LockType::EXCLUSIVE};  // synchronized section
        if (get_file_size(fid) == 0) {
            if (ftruncate(fid, mapsize) != 0) {
                printf("ERROR ftruncate\n");
                return 1;
            }
            a = new (ptr) A(0);
        } else {
            a = reinterpret_cast<A*>(ptr);
        }

        // using an atomic to share data through a shared_memory is only safe if the atomic is lock-free :
        if (!a->internal_value.is_lock_free()) {
            printf("ERROR : the atomic is not lock-free, we cannot use it through shared_memory\n");
            return 1;
        }

        printf("Beginning : initial_state = %d\n", a->read());

        // EXTRAIT = After that we read our value, we need increment it while the file is still locked.
        // NdM : le read puis write n'étant pas atomique, il faut synchroniser avec le lock.
        value = a->read();
        value += 1;
        a->write(value);

        // EXTRAIT = Once we have atomically incremented our variable, we can unlock the file and allow other processes
        // to access it.

    }  // end of synchronized section

    // EXTRAIT = At this point we can use the struct as if it was created in a multi-threaded application. In this toy
    // example we do a ping pong where one application increments the variable and waits for it to change while the
    // other does the same.

    for (int j = 0; j < 3; ++j) {
        // NdM : ici, les watch + write ne sont PAS synchronisés ! Pire : on "incrémente" pas par rapport à la valeur
        // dans l'atomic, mais par rapport à l'état local de chaque process.
        //
        // En théorie, on a une race condition (mais pas de data-race grâce à l'atomic). En pratique, ça marchouille car
        // comme il n'y a que deux process qui écrivent, lorsque l'un watche, l'autre est le seul à écrire, et il ajoute
        // deux sans se soucier de ce qu'a écrit l'autre...
        //
        // Dit autrement, de façon contre-intuitive, les process ne se contentent pas d'incrémenter la valeur partagée,
        // mais uniquement de modifier par rapport à leur état interne. C'est très visible si on incrément de 10 au lieu
        // de 2 : chaque process incrémentant 3 fois, on s'attendrait à ce que l'état final soit augmenté de 60 (plus
        // les deux incréments dans le lock). Or, on observe un incrément final de 30 uniquement : chaque process a
        // écrit sa valeur en ignorant le travail de l'autre.
        // Conclusion : partager un atomic dans une shared-memory ne nous dispense pas de synchroniser les read / write
        printf("-->proc %d %d\n", getpid(), value);
        a->watch(value);
        value += 2;
        a->write(value);

        /* // En alternative, on peut utiliser les opérations de l'atomic pour incrémenter correctement : */
        /* printf("-->proc %d %d\n", getpid(), a->internal_value.load()); */
        /* a->internal_value += 1; */
        /* usleep(1000);  // help OS to alternate between processes */
    }
    printf("Finishing : final_state = %d\n", a->read());
    ::munmap(ptr, mapsize);
    ::close(fid);
}

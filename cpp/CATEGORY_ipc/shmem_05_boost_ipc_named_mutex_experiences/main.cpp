#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <dirent.h>
#include <cstring>

#include <boost/interprocess/sync/named_mutex.hpp>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = analyse du fonctionnemnet des boost::interprocess::named_mutex

CE QUE FAIT LA POC :
    le présent binaire 'mutex_god' permet d'effectuer plusieurs actions sur un named_mutex :
        vérification = le mutex exist-t-il ? Si oui, est-il locké ?
        locker le mutex puis aborter
        forcer la suppression du mutex
    la POC enchaîne plusieurs étapes :
        clean préliminaire pour partir d'un état propre
        lock puis abort
        vérification de l'état du mutex
        force remove
        vérification de l'état du mutex

CONCLUSIONS :

Constaté :
    un named_mutex a une existence INDÉPENDANTE des process qui l'utilisent
    un process peut tout à fait locker un named_mutex, puis quitter sans le délocker
    on se retrouve alors dans un état où le named_mutex existe et est locké (!) alors même qu'aucun process ne l'utilise
    (d'où la nécessité de partir d'un état propre, ce qu'on peut faire a priori facilement grâce au comportement de sem_unlink, qui n'invalide pas les sémaphores existants, tout en permettant de réutiliser le nom pour un nouveau sémaphore)

Implémentation :
    sur Linux, les mutex IPC semblent implémentés avec les sémaphores POSIX
    du coup, un named_mutex qui existe apparaîtra dans :
        /dev/shm/sem.my_super_mutex
    (c'est un moyen de savoir si un mutex existe sans passer par un programme utilisant boost::ipc)
    la méthode statique named_mutex::remove appelle probablement shm_unlink, qui a un comportement particulier :
        https://www.systutorials.com/docs/linux/man/3p-sem_unlink/
        man sem_unlink
        The sem_unlink() function shall remove the semaphore named by the string name.
            un process tiers peut supprimer un mutex créé par un précédent process
        If the semaphore named by name is currently referenced by other processes, then sem_unlink() shall have no effect on the state of the semaphore.
        If one or more processes have the semaphore open when sem_unlink() is called, destruction of the semaphore is postponed until all references to the semaphore have been destroyed by calls to sem_close(), _exit(), or exec.
        Calls to sem_open() to recreate or reconnect to the semaphore refer to a new semaphore after sem_unlink() is called.
            une fois qu'on a appelé sem_unlink, tout nouveau process peut "réutiliser le nom" car il pointera sur un AUTRE sémaphore
            et les anciens process qui utilisaient encore l'ancien sémaphore peuvent également continuer de le faire de façon safe
        The sem_unlink() call shall not block until all references have been destroyed; it shall return immediately.
            dans tous les cas, sem_unlink ne bloque pas : il se contente éventuellement de retarder la suppression effective du mutex

À partir de là, quelles bonnes pratiques je suis :
    Sous ces hypothèses, je peux bien attaquer toute nouvelle POC avec un 'remove' préliminaire :
        il n'aura pas d'impact si le mutex n'existe pas (il se contentera de renvoyer false)
        si le mutex existe et n'est utilisé par personne, il remets les choses au clair
        si le mutex existe et est utilisé par des process, ils pourront continuer de l'utiliser
    Le seul hic, c'est que les anciens process risquent fort d'appeler 'remove' à leur tour, ce qui n'invalidera non pas leur ancien mutex, mais le nouveau !


)DELIM";
    std::cout << std::endl;
}

namespace bipc = boost::interprocess;

static const std::string LOCK_THEN_ABORT = "lock_then_abort";
static const std::string CHECK_MUTEX = "check_mutex";
static const std::string FORCE_REMOVE = "force_remove";
static const std::string DESCRIPTION = "description";
static const std::vector<std::string> commands{LOCK_THEN_ABORT, FORCE_REMOVE, DESCRIPTION};

void print_usage(std::string argv0) {
    std::cout << "USAGE :" << std::endl;
    std::cout << "\t" << argv0 << "  <COMMAND>  <MUTEX_NAME>" << std::endl;
    std::cout << "Where COMMAND is one of :" << std::endl;
    for (auto command : commands) {
        std::cout << "\t" << command << std::endl;
    }
    std::cout << "EXAMPLE :" << std::endl;
    std::cout << "\t" << argv0 << "  is_existing  my_super_mutex" << std::endl;
}

void list_directory(std::string dirname) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(dirname.c_str())) != NULL) {
        /* print all the files and directories within directory */
        printf("Content of %s :\n", dirname.c_str());
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;  // ignoring "." and ".."
            }
            printf("    %s%s\n", dirname.c_str(), ent->d_name);
        }
        printf("\n");
        closedir(dir);
    } else {
        std::ostringstream oss;
        oss << "Unable to open directory '" << dirname << "' : " << strerror(errno) << std::endl;
        throw std::runtime_error(oss.str());
    }
}

void lock_then_abort(std::string mutex_name) {
    bipc::named_mutex the_mutex(bipc::open_or_create, mutex_name.c_str());
    std::cout << "Trying to lock interprocess mutex with name '" << mutex_name << "'" << std::endl;
    the_mutex.lock();
    std::cout << "Mutex is locked ; I will now abort..." << std::endl;
    // aborting will terminate program WITHOUT unlocking the mutex (which is what we want here)
    std::abort();
}

bool is_existing(std::string mutex_name) {
    // on se contente de vérifier si le mutex existe
    // notamment, pas de création de mutex ici (inutile, donc, d'appeler remove)
    // notamment, pas de lock de mutex ici (inutile, donc, d'appeler unlock)
    try {
        bipc::named_mutex the_mutex(bipc::open_only, mutex_name.c_str());
        return true;
    } catch (bipc::interprocess_exception& e) {
        return false;
    }
}

bool is_locked(std::string mutex_name) {
    // precondition = the mutex exists (or else we raise)
    bipc::named_mutex the_mutex(bipc::open_only, mutex_name.c_str());
    try {
        bool is_currently_unlocked = the_mutex.try_lock();
        the_mutex.unlock();
        return !is_currently_unlocked;
    } catch (bipc::interprocess_exception& e) {
        std::cout << "ERROR : caught ipc exception : " << e.what() << std::endl;
        the_mutex.unlock();
        throw;
    }
}

void force_remove(std::string mutex_name) {
    bool returned = bipc::named_mutex::remove(mutex_name.c_str());
    std::cout << "Removed interprocess mutex with name '" << mutex_name << "'  ";
    std::cout << "(returned=" << std::boolalpha << returned << ")" << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        throw std::runtime_error("missing mandatory command, see USAGE");
    }

    const std::string command = argv[1];
    const std::string mutex_name = argv[2];
    std::cout << "MUTEX GOD> running command '" << command << "' on mutex '" << mutex_name << "'" << std::endl;

    if (command == LOCK_THEN_ABORT) {
        lock_then_abort(mutex_name);
    } else if (command == CHECK_MUTEX) {
        bool exists = is_existing(mutex_name);
        if (!exists) {
            std::cout << "mutex '" << mutex_name << "' DOES NOT EXIST" << std::endl;
        } else {
            // ici, on suppose que le mutex existe (et on ignore la race-condition TOCTOU)
            std::cout << "mutex '" << mutex_name;
            std::cout << "' EXISTS AND " << (is_locked(mutex_name) ? "IS" : "IS NOT") << " LOCKED" << std::endl;
        }
        list_directory("/dev/shm/");
    } else if (command == FORCE_REMOVE) {
        force_remove(mutex_name);
    } else if (command == DESCRIPTION) {
        print_poc_description();
        print_usage(argv[0]);
    } else {
        print_usage(argv[0]);
        throw std::runtime_error("invalid command, see USAGE");
    }
}

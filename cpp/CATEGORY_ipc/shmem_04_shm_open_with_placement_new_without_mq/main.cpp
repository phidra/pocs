#include <dirent.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "shared_mem.h"
#include "utils.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = POC complète de shared-memory pour IPC, avec placement-new pour construire le payload.
    il va y avoir trois process :
        un process parent qui forke le producer et le consumer (responsable de construire puis détruire le payload)
        un producer (qui écrit sur la shared-mem)
        un consumer (qui lit la shared-mem)
    les sous-process se synchronisent avec des flags :
        quand le producer écrit, le consumer est bloqué
        ça n'est que quand le producer a fini d'écrire qu'il débloque le consumer
        quand le consumer lit, le producer est bloqué
        ça n'est que quand le consumer a fini de lire qu'il débloque le producer
    comment s'assure-t-on que les sous-process n'utilisent pas la shared-mem/le Payload avant qu'il soit construit ?
        car le parent ne forke les sous-process qu'après avoir créé la shared-mem et utilisé placement-new dessus
    comment s'assure-t-on que le parent ne détruit le Payload/la shared-mem que lorsque plus personne ne l'utilise ?
        les sous-process signalent qu'ils en ont fini avec le Payload avec des flags dans le payload
    (c'est la POC qui forke les sous-process, inutile de lancer le binaire plusieurs fois)

C'est la même POC que la POC 3, mais sans les MQ POSIX, donc beaucoup plus simple !
Dans la POC 3, on utilisait des MQ POSIX pour éviter que le process parent ne détruise le Payload (et appelle shm_unlink)
alors que le payload était encore utilisé par les process enfants.
Dans la présente POC, on utilise plutôt directement des flags dans le payload pour ça.
(et le payload était construit dans la shared-mem par le process parent AVANT qu'il ne forke les sous-process,
donc il n'y a pas de risque que les sous-process utilisent la shared-mem avant que le Payload ne soit construit)

)DELIM";
    std::cout << std::endl;
}

struct Payload {
    std::atomic_bool consumer_has_work_to_do;
    std::atomic_bool producer_has_work_to_do;
    std::atomic_bool consumer_is_still_using_the_payload;
    std::atomic_bool producer_is_still_using_the_payload;
    char shared_data;

    Payload() :
        consumer_has_work_to_do{false},
        producer_has_work_to_do{false},
        consumer_is_still_using_the_payload{true},
        producer_is_still_using_the_payload{true} {
        std::cout << "CONSTRUCTOR CALLED !" << std::endl;
    }

    ~Payload() { std::cout << "DESTRUCTOR CALLED !" << std::endl; }
};

void producer(SharedMem<Payload> const& writer) {
    Payload& payload = writer.get();

    if (!payload.consumer_has_work_to_do.is_lock_free()) {
        throw std::runtime_error("Flag is not lock-free");
    }

    char data_to_send[] = "SKYWALKER";  // EOF signals the end of transmission
    std::cout << "PRODUCER will send this message : " << data_to_send << "\n" << std::endl;
    for (size_t i = 0; i < strlen(data_to_send); i++) {
        payload.shared_data = data_to_send[i];
        payload.consumer_has_work_to_do.store(true);

        payload.producer_has_work_to_do.store(false);
        while (!payload.producer_has_work_to_do.load()) {}
    }

    std::cout << "=== End of emission, PRODUCER will now close ===" << std::endl;

    // Ici, le producer n'a plus rien à faire, il prévient le process parent qu'il n'a plus l'usage du payload :
    payload.producer_is_still_using_the_payload.store(false);
}

void consumer(SharedMem<Payload> const& reader) {
    Payload& payload = reader.get();

    if (!payload.consumer_has_work_to_do.is_lock_free()) {
        throw std::runtime_error("Flag is not lock-free");
    }

    char received_char;

    do {
        while (!payload.consumer_has_work_to_do.load()) {}
        payload.consumer_has_work_to_do.store(false);

        received_char = payload.shared_data;
        if (received_char == '') {
            std::cout << "=== End of reception, CONSUMER will now close ===" << std::endl;
        } else {
            std::cout << "CONSUMER got new data : " << payload.shared_data << std::endl;
        }

        payload.producer_has_work_to_do.store(true);
    } while (received_char != '');

    // Ici, le consumer n'a plus rien à faire, il prévient le process parent qu'il n'a plus l'usage du payload :
    payload.consumer_is_still_using_the_payload.store(false);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    std::string shared_memory_name = "/mysupersharedmemory";

    std::cout << "\n---\n--- STEP 2 = creating shared-memory (before fork -> all child processes will share the same "
                 "fd and the same mmapped region)"
              << std::endl;

    SharedMem<Payload> shmem{shared_memory_name};
    // Comprendre comment est partagée cette structure entre un process parent et son fils lors de l'appel à fork
    // est très important, cf. les commentaires dans la POC précédente.

    if (fork() == 0) {
        // CHILD 1 :
        std::cout << "\n---\n--- STEP 3 = lancement du CONSUMER dans un subprocess" << std::endl;
        consumer(shmem);
    } else {
        if (fork() == 0) {
            // CHILD 2 :
            std::cout << "\n---\n--- STEP 4 = lancement du PRODUCER dans un subprocess" << std::endl;
            producer(shmem);
        } else {
            // PARENT :
            std::cout << "\n---\n--- STEP 5 = le process PARENT attend l'aval des deux sous-process pour libérer les "
                         "ressources"
                      << std::endl;

            // On ne doit appeler le destructeur ~Payload (et shm_unlink) qu'une seule fois. Comment ?
            // Seule la SharedMem<PayLoad> du parent est owner, et elle fait ces opérations spéciales à sa destruction.
            shmem.set_owner();

            // Pour être bien sûr que plus personne ne se sert du Payload, on attend de recevoir les signaux des process
            Payload& payload = shmem.get();
            while (payload.consumer_is_still_using_the_payload.load() ||
                   payload.producer_is_still_using_the_payload.load()) {
                // attente bloquante... on pourrait pousser plus loin en utilisant une condition-variable pthread pour
                // réveiller le process parent lorsqu'il peut détruire la shared-mem, mais je préfère rester simple.
            }

            // Juste histoire de mieux comprendre ce qui se passe, j'inspecte les répertoires /dev :
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            std::cout << "\n---\n--- STEP 6 = avant de libérer les ressources, insepction des répertoires pertinents "
                         "de /dev :"
                      << std::endl;
            std::cout << std::endl;
            list_directory("/dev/shm/");
            list_directory("/dev/mqueue/");

            std::cout << "\n---\n--- STEP 7 = libération des ressources" << std::endl;
            // À ce stade, comme on s'apprête à sortir du scope, le destructeur ~SharedMem va être appelé.
            // Comme cette SharedMem est owner, elle appellera (manuellement !) le destructeur ~Payload sur la
            // zone mémoire de la shared-memory.
            // De plus, elle appellera shm_unlink.
        }
    }
}

#include <atomic>
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = équivalent strict de la POC 04 utilisant boost::interprocess:shared_memory_object
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

La différence avec la POC 04, c'est qu'au lieu d'utiliser shm_open/shm_unlink/mmap, on utilise boost::ipc

Ici, je "triche" un peu car les process sont forkés *après* mmapping (et se partagent donc le mapping déjà créé).
Je vais faire une autre POC où chaque process fait son propre mapping.

)DELIM";
    std::cout << std::endl;
}

struct Payload {
    std::atomic_bool consumer_has_work_to_do;
    std::atomic_bool producer_has_work_to_do;
    std::atomic_bool consumer_is_still_using_the_payload;
    std::atomic_bool producer_is_still_using_the_payload;
    char shared_data;

    Payload()
        : consumer_has_work_to_do{false},
          producer_has_work_to_do{false},
          consumer_is_still_using_the_payload{true},
          producer_is_still_using_the_payload{true} {
        std::cout << "CONSTRUCTOR CALLED !" << std::endl;
    }

    ~Payload() { std::cout << "DESTRUCTOR CALLED !" << std::endl; }
};

void producer(boost::interprocess::mapped_region& region) {
    Payload& payload = *(static_cast<Payload*>(region.get_address()));

    if (!payload.consumer_has_work_to_do.is_lock_free()) {
        throw std::runtime_error("Flag is not lock-free");
    }

    char data_to_send[] = "SKYWALKER";  // EOF signals the end of transmission
    std::cout << "PRODUCER will send this message : " << data_to_send << "\n" << std::endl;
    for (size_t i = 0; i < strlen(data_to_send); i++) {
        payload.shared_data = data_to_send[i];
        payload.consumer_has_work_to_do.store(true);

        payload.producer_has_work_to_do.store(false);
        while (!payload.producer_has_work_to_do.load()) {
        }
    }

    std::cout << "=== End of emission, PRODUCER will now close ===" << std::endl;

    // Ici, le producer n'a plus rien à faire, il prévient le process parent qu'il n'a plus l'usage du payload :
    payload.producer_is_still_using_the_payload.store(false);
}

void consumer(boost::interprocess::mapped_region& region) {
    Payload& payload = *(static_cast<Payload*>(region.get_address()));

    if (!payload.consumer_has_work_to_do.is_lock_free()) {
        throw std::runtime_error("Flag is not lock-free");
    }

    char received_char;

    do {
        while (!payload.consumer_has_work_to_do.load()) {
        }
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

    using namespace boost::interprocess;

    std::cout << "\n---\n--- STEP 2 = creating shared-memory (before fork -> all child processes will share the same "
                 "fd and the same mmapped region)"
              << std::endl;

    // Create a shared memory object.
    shared_memory_object shm_obj(create_only, "my_super_shared_memory", read_write);

    // Set size
    shm_obj.truncate(sizeof(Payload));

    // Map the whole shared memory in this process
    mapped_region region(shm_obj, read_write);

    // construct object in mmapped memory :
    Payload* ptr = new (region.get_address()) Payload{};

    if (fork() == 0) {
        // CHILD 1 :
        std::cout << "\n---\n--- STEP 3 = lancement du CONSUMER dans un subprocess" << std::endl;
        consumer(region);
    } else {
        if (fork() == 0) {
            // CHILD 2 :
            std::cout << "\n---\n--- STEP 4 = lancement du PRODUCER dans un subprocess" << std::endl;
            producer(region);
        } else {
            // PARENT :
            std::cout << "\n---\n--- STEP 5 = le process PARENT attend l'aval des deux sous-process pour libérer les "
                         "ressources"
                      << std::endl;

            // Pour être bien sûr que plus personne ne se sert du Payload, on attend de recevoir les signaux des process
            Payload& payload = *ptr;
            while (payload.consumer_is_still_using_the_payload.load() ||
                   payload.producer_is_still_using_the_payload.load()) {
                // attente bloquante... on pourrait pousser plus loin en utilisant une condition-variable pthread pour
                // réveiller le process parent lorsqu'il peut détruire la shared-mem, mais je préfère rester simple.
            }
            shared_memory_object::remove("my_super_shared_memory");
        }
    }
}

#include <iostream>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

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

namespace bipc = boost::interprocess;

static const char* CONSUMER_MUST_WORK_MUTEX = "consumer_must_work_mutex";
static const char* CONSUMER_MUST_WORK_CV = "consumer_must_work_cv";
static const char* PRODUCER_MUST_WORK_MUTEX = "producer_must_work_mutex";
static const char* PRODUCER_MUST_WORK_CV = "producer_must_work_cv";
static const char* SYNCHRONIZATION_QUEUE = "synchronization_queue";
static const char* PAYLOAD = "my_payload";

struct Payload {
    char shared_data;
    bool producer_must_work_is_notified;
    bool consumer_must_work_is_notified;

    Payload()
        :  // le Payload commence avec un état déterminé = le shared_data est un caractère nul
          shared_data{'\0'},
          producer_must_work_is_notified{false},
          consumer_must_work_is_notified{false} {
        std::cout << "CONSTRUCTOR CALLED !" << std::endl;
    }

    ~Payload() { std::cout << "DESTRUCTOR CALLED !" << std::endl; }
};

void producer() {
    // construct payload :
    bipc::shared_memory_object shm_obj(bipc::create_only, PAYLOAD, bipc::read_write);
    shm_obj.truncate(sizeof(Payload));
    bipc::mapped_region region(shm_obj, bipc::read_write);
    Payload* ptr = new (region.get_address()) Payload{};
    Payload& payload = *ptr;

    // in order for the consumer not to do anything until the producer has properly constructed the Payload,
    // we use a message-queue to signal that the producer has constructed the payload :
    {
        constexpr const std::size_t max_num_msg = 1;   // we need only 1 message for our synchronization
        constexpr const std::size_t max_msg_size = 0;  // an empty message is enough for our synchronization
        bipc::message_queue mq(bipc::open_or_create, SYNCHRONIZATION_QUEUE, max_num_msg, max_msg_size);
        constexpr const unsigned int priority = 0;
        std::cout << "PRODUCER> sending on mq" << std::endl;
        mq.send(nullptr, 0, priority);  // sending an empty message in the queue
        std::cout << "PRODUCER> sent on mq" << std::endl;

        // ATTENTION : ici il ne faut pas remove la MQ !
        // (sans quoi on prend le risque de le faire AVANT que le consumer ne l'ait dépilée)
        // En revanche, on peut laisser détruire la bipc::message_queue, car le producer n'en a plus besoin.
        // (cette destruction de la bipc::message_queue locale ne supprime pas la ressource IPC partagée : elle se
        // contente d'indiquer que le process courant ne l'utilise plus)
    }

    bipc::named_mutex producer_must_work_mutex(bipc::open_or_create, PRODUCER_MUST_WORK_MUTEX);
    bipc::named_condition producer_must_work_cv(bipc::open_or_create, PRODUCER_MUST_WORK_CV);
    bipc::named_mutex consumer_must_work_mutex(bipc::open_or_create, CONSUMER_MUST_WORK_MUTEX);
    bipc::named_condition consumer_must_work_cv(bipc::open_or_create, CONSUMER_MUST_WORK_CV);

    char data_to_send[] = "SKYWALKER";  //  (=EOF) signals the end of transmission
    size_t nb_chars_to_send = strlen(data_to_send);
    std::cout << "PRODUCER will send this message : " << data_to_send << "\n" << std::endl;

    // en boucle, on va : 1. envoyer le caractère, et 2. attendre que le consumer nous dise d'envoyer le suivant :
    for (size_t i = 0; i < nb_chars_to_send; i++) {
        // STEP 1 = envoyer le caractère :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(consumer_must_work_mutex);
            payload.shared_data = data_to_send[i];

            // histoire que les CV servent à quelque chose, j'ajoute une attente artificielle avant le notify :
            std::this_thread::sleep_for(0.1s);
            payload.consumer_must_work_is_notified = true;
            consumer_must_work_cv.notify_one();

            std::cout << "PRODUCER> sent : '" << payload.shared_data << "'" << std::endl;
        }

        // STEP 2 = attendre que le consumer nous dise qu'on peut envoyer le suivant :
        if (data_to_send[i] != '')  // si on n'a plus rien à envoyer, inutile d'attendre :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(producer_must_work_mutex);
            while (!payload.producer_must_work_is_notified) {
                producer_must_work_cv.wait(lock);
            }

            // ici, on est débloqués, c'est qu'on a du travail à faire (on peut reset le bool) :
            payload.producer_must_work_is_notified = false;
        }
    }

    std::cout << "PRODUCER> exiting here" << std::endl;
}

void consumer() {
    // in order for the consumer not to do anything until the producer has properly constructed the Payload,
    // we use a message-queue to signal that the producer has constructed the payload :
    {
        struct mq_remove {
            // la MQ a joué son rôle de synchro, elle peut être remove dès la sortie du présent scope
            // (par le consumer uniquement, car si le producer la remove, il risque de le faire avant que le consumer
            // ait eu le temps de la dépiler)
            ~mq_remove() { bipc::message_queue::remove(SYNCHRONIZATION_QUEUE); }
        } mq_remover;
        constexpr const std::size_t max_num_msg = 1;   // we need only 1 message for our synchronization
        constexpr const std::size_t max_msg_size = 0;  // an empty message is enough for our synchronization
        // on crée la MQ en open_or_create pour le cas où le consumer s'exécute avant le producer :
        bipc::message_queue mq(bipc::open_or_create, SYNCHRONIZATION_QUEUE, max_num_msg, max_msg_size);
        unsigned int priority = 0;
        std::size_t received_size = 0;
        std::cout << "CONSUMER> receiving on mq" << std::endl;
        mq.receive(nullptr, 0, received_size, priority);
        std::cout << "CONSUMER> received on mq" << std::endl;
    }

    // la shared-mem est créée en open_only, car on SAIT que le producer a pu la créer (grâce à la queue de synchro) :
    bipc::shared_memory_object shm_obj(bipc::open_only, PAYLOAD, bipc::read_write);
    bipc::mapped_region region(shm_obj, bipc::read_write);
    // pas de placement-new côté consumer : le Payload a DÉJÀ été construit par le producer :
    Payload* ptr = static_cast<Payload*>(region.get_address());
    Payload& payload = *ptr;
    // en revanche, on détruit le Payload côté consumer (qui est le dernier à s'en servir) :
    struct payload_destruction {
        payload_destruction(Payload* ptr_) : ptr{ptr_} {}
        ~payload_destruction() { ptr->~Payload(); }
        Payload* ptr;
    } payload_destructor{ptr};

    bipc::named_mutex producer_must_work_mutex(bipc::open_or_create, PRODUCER_MUST_WORK_MUTEX);
    bipc::named_condition producer_must_work_cv(bipc::open_or_create, PRODUCER_MUST_WORK_CV);
    bipc::named_mutex consumer_must_work_mutex(bipc::open_or_create, CONSUMER_MUST_WORK_MUTEX);
    bipc::named_condition consumer_must_work_cv(bipc::open_or_create, CONSUMER_MUST_WORK_CV);

    char received_char = 0;  // n'importe quel caractère différent de '' convient ici
    do {
        // STEP 1 = on attend l'envoi d'un caractère :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(consumer_must_work_mutex);
            while (!payload.consumer_must_work_is_notified) {
                consumer_must_work_cv.wait(lock);
            }
            payload.consumer_must_work_is_notified = false;
        }

        // ici, on est débloqués, c'est qu'on a du travail à faire
        // On sait que le producer est bloqué en attente de notre signal (il se contente éventuellement de lire le bool
        // 'producer_must_work_is_notified', mais n'écrira rien)
        // On peut donc lire la donnée sans la protection de l'un de nos mutex :
        received_char = payload.shared_data;
        if (received_char == '') {
            std::cout << "=== End of reception, CONSUMER will now close ===" << std::endl;
        } else {
            std::cout << "CONSUMER got new data : '" << payload.shared_data << "'" << std::endl;
        }

        // STEP 2 = on signale le producer :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(producer_must_work_mutex);
            // histoire que les CV servent à quelque chose, j'ajoute une attente artificielle avant le notify :
            std::this_thread::sleep_for(0.2s);
            payload.producer_must_work_is_notified = true;
            producer_must_work_cv.notify_one();
        }

    } while (received_char != '');

    std::cout << "CONSUMER> exiting here" << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // C'est le seul truc que le parent a besoin de faire = faire un clean préalable :
    bipc::message_queue::remove(SYNCHRONIZATION_QUEUE);
    bipc::shared_memory_object::remove(PAYLOAD);
    bipc::named_mutex::remove(CONSUMER_MUST_WORK_MUTEX);
    bipc::named_condition::remove(CONSUMER_MUST_WORK_CV);
    bipc::named_mutex::remove(PRODUCER_MUST_WORK_MUTEX);
    bipc::named_condition::remove(PRODUCER_MUST_WORK_CV);

    if (fork() != 0) {  // this is the parent process from the fork (but this is irrelevant here)
        // décommenter cette ligne pour vérifier que tout est OK même si le consumer démarre après le producer :
        /* std::this_thread::sleep_for(2s); */

        consumer();
        // Si on arrive ici, c'est que le consumer a reçu le caractère '' (EOF) signalant la fin de la transmission.
        // Or, c'est la dernière action qu'a effectuée le producer avant de terminate.
        // Le consumer peut donc supprimer les ressources IPC qui ont été utilisées, on est sûr que le producer ne les
        // utilise plus. (éventuellement, si le producer n'a pas encore quitté, et que les destructeurs sont encore en
        // train de s'exécuter, on bénéficiera du mécanisme décrit dans shm_unlink = la destruction de la ressource sera
        // effective quand le producer aura terminé de détruire ses objets locaux l'utilisant).
        bipc::shared_memory_object::remove(PAYLOAD);
        bipc::named_mutex::remove(CONSUMER_MUST_WORK_MUTEX);
        bipc::named_condition::remove(CONSUMER_MUST_WORK_CV);
        bipc::named_mutex::remove(PRODUCER_MUST_WORK_MUTEX);
        bipc::named_condition::remove(PRODUCER_MUST_WORK_CV);
    } else {  // this is the child process from the fork (but this is irrelevant here)
        // décommenter cette ligne pour vérifier que tout est OK même si le producer démarre après le consumer :
        /* std::this_thread::sleep_for(2s); */

        producer();
    }
}

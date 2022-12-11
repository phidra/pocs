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
CE QUE MONTRE CETTE POC = équivalent de la POC 06 en essayant de simplifier la POC pour utiliser moins de ressources interprocess :


)DELIM";
    std::cout << std::endl;
}

namespace bipc = boost::interprocess;

static const char* MUTEX_NAME = "shared_mutex";
static const char* CV_NAME = "shared_cv";
static const char* SYNCHRONIZATION_QUEUE = "synchronization_queue";
static const char* PAYLOAD = "my_payload";

struct Payload {
    char shared_data;

    Payload()
        :  // le Payload commence avec un état déterminé = le shared_data est un caractère nul
          shared_data{'\0'} {
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

    bipc::named_mutex the_mutex(bipc::open_or_create, MUTEX_NAME);
    bipc::named_condition the_cv(bipc::open_or_create, CV_NAME);

    char data_to_send[] = "SKYWALKER";  //  (=EOF) signals the end of transmission
    size_t nb_chars_to_send = strlen(data_to_send);
    std::cout << "PRODUCER will send this message : " << data_to_send << "\n" << std::endl;

    // en boucle, on va : 1. envoyer le caractère, et 2. attendre que le consumer nous dise d'envoyer le suivant :
    for (size_t i = 0; i < nb_chars_to_send; i++) {
        // STEP 1 = envoyer le caractère :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(the_mutex);
            payload.shared_data = data_to_send[i];

            // histoire que les CV servent à quelque chose, j'ajoute une attente artificielle avant le notify :
            std::this_thread::sleep_for(0.1s);
            the_cv.notify_one();

            std::cout << "PRODUCER> sent : '" << payload.shared_data << "'" << std::endl;
        }

        // STEP 2 = attendre que le consumer nous dise qu'on peut envoyer le suivant :
        if (data_to_send[i] != '')  // si on n'a plus rien à envoyer, inutile d'attendre :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(the_mutex);
            while (payload.shared_data != '\0') {
                // tant que le caractère est toujours non-nul, c'est que le consumer ne nous a pas signalés
                the_cv.wait(lock);
            }
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

    bipc::named_mutex the_mutex(bipc::open_or_create, MUTEX_NAME);
    bipc::named_condition the_cv(bipc::open_or_create, CV_NAME);

    char received_char = 0;  // n'importe quel caractère différent de '' convient ici
    do {
        // STEP 1 = on attend l'envoi d'un caractère :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(the_mutex);
            while (payload.shared_data == '\0') {
                the_cv.wait(lock);
            }
        }

        // ici, on est débloqués, c'est qu'on a du travail à faire
        // On sait que le producer restera bloqué tant que le caractère ne sera pas nul
        // On peut donc lire la donnée sans la protection de l'un de nos mutex :
        received_char = payload.shared_data;
        if (received_char == '') {
            std::cout << "=== End of reception, CONSUMER will now close ===" << std::endl;
        } else {
            std::cout << "CONSUMER got new data : '" << payload.shared_data << "'" << std::endl;
        }

        // STEP 2 = on signale le producer :
        {
            bipc::scoped_lock<bipc::named_mutex> lock(the_mutex);
            // histoire que les CV servent à quelque chose, j'ajoute une attente artificielle avant le notify :
            std::this_thread::sleep_for(0.2s);
            payload.shared_data = '\0';  // setter shared_data à zéro permet de prévenir le producer qu'il a du taf
            the_cv.notify_one();
        }

    } while (received_char != '');

    std::cout << "CONSUMER> exiting here" << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // C'est le seul truc que le parent a besoin de faire = faire un clean préalable :
    bipc::message_queue::remove(SYNCHRONIZATION_QUEUE);
    bipc::shared_memory_object::remove(PAYLOAD);
    bipc::named_mutex::remove(MUTEX_NAME);
    bipc::named_condition::remove(CV_NAME);
    bipc::named_mutex::remove(MUTEX_NAME);
    bipc::named_condition::remove(CV_NAME);

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
        bipc::named_mutex::remove(MUTEX_NAME);
        bipc::named_condition::remove(CV_NAME);
        bipc::named_mutex::remove(MUTEX_NAME);
        bipc::named_condition::remove(CV_NAME);
    } else {  // this is the child process from the fork (but this is irrelevant here)
        // décommenter cette ligne pour vérifier que tout est OK même si le producer démarre après le consumer :
        /* std::this_thread::sleep_for(2s); */

        producer();
    }
}

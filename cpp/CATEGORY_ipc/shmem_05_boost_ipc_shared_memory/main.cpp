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

static const char* CONSUMER_MUST_WORK_MUTEX = "consumer_must_work_mutex";
static const char* CONSUMER_MUST_WORK_CV = "consumer_must_work_cv";

struct Payload {
    char shared_data;
    bool producer_must_work_bool;
    bool consumer_must_work_bool;

    Payload()
        :  // le Payload commence avec un état déterminé = le shared_data est un caractère nul
          shared_data{'\0'},
          producer_must_work_bool{false},
          consumer_must_work_bool{false} {
        std::cout << "CONSTRUCTOR CALLED !" << std::endl;
    }

    ~Payload() { std::cout << "DESTRUCTOR CALLED !" << std::endl; }
};

void producer() {
    using namespace boost::interprocess;

    // in order for the consumer not to do anything until the producer has properly constructed the Payload,
    // we use a message-queue to signal that the producer has constructed the payload :
    struct mq_remove {
        ~mq_remove() { message_queue::remove("payload_is_constructed"); }
    } mq_remover;

    struct shm_remove {
        shm_remove() { shared_memory_object::remove("my_payload"); }
        ~shm_remove() { shared_memory_object::remove("my_payload"); }
    } shm_remover;

    constexpr const std::size_t max_num_msg = 1;   // we need only 1 message for our synchronization
    constexpr const std::size_t max_msg_size = 0;  // an empty message is enough for our synchronization
    message_queue mq(open_or_create, "payload_is_constructed", max_num_msg, max_msg_size);

    if (mq.get_num_msg() != 0) {
        std::cout << "PROBLEME ICI TODO" << std::endl;
        std::cout << "PROBLEME ICI TODO" << std::endl;
        std::cout << "PROBLEME ICI TODO" << std::endl;
        std::cout << "PROBLEME ICI TODO" << std::endl;
    }

    // construct payload :
    shared_memory_object shm_obj(create_only, "my_payload", read_write);
    shm_obj.truncate(sizeof(Payload));
    mapped_region region(shm_obj, read_write);
    Payload* ptr = new (region.get_address()) Payload{};
    Payload& payload = *ptr;
    std::cout << payload.shared_data << std::endl;

    // here, payload is properly constructed, we can signal the consumer, and remove queue :
    constexpr const unsigned int priority = 0;
    std::cout << "PRODUCER> sending on mq" << std::endl;
    std::this_thread::sleep_for(1s);
    mq.send(nullptr, 0, priority);  // sending an empty message in the queue
    std::cout << "PRODUCER> sent on mq" << std::endl;

    named_mutex consumer_must_work_mutex(open_or_create, CONSUMER_MUST_WORK_MUTEX);
    named_condition consumer_must_work_cv(open_or_create, CONSUMER_MUST_WORK_CV);

    char data_to_send[] = "SKYWALKER";  // EOF signals the end of transmission
    std::cout << "PRODUCER will send this message : " << data_to_send << "\n" << std::endl;

    {
        std::cout << "PRODUCER> about to lock" << std::endl;
        scoped_lock<named_mutex> lock(consumer_must_work_mutex);
        payload.shared_data = data_to_send[0];
        std::cout << "PRODUCER> sent : '" << payload.shared_data << "'" << std::endl;
        std::this_thread::sleep_for(1s);
        payload.consumer_must_work_bool = true;
        std::cout << "PRODUCER> before notifying consumer, 'consumer_must_work_bool' is " << std::boolalpha
                  << payload.consumer_must_work_bool << std::endl;
        consumer_must_work_cv.notify_one();
        std::cout << "PRODUCER> after  notifying consumer, 'consumer_must_work_bool' is " << std::boolalpha
                  << payload.consumer_must_work_bool << std::endl;
    }

    // Ici, le consumer nous a rendu la main, on quitte
    std::this_thread::sleep_for(1s);
    std::cout << "PRODUCER> quitting here" << std::endl;
}

void consumer() {
    using namespace boost::interprocess;

    // in order for the consumer not to do anything until the producer has properly constructed the Payload,
    // we use a message-queue to signal that the producer has constructed the payload :
    struct mq_remove {
        ~mq_remove() { message_queue::remove("payload_is_constructed"); }
    } mq_remover;

    struct shm_remove {
        shm_remove() { shared_memory_object::remove("my_payload"); }
        ~shm_remove() { shared_memory_object::remove("my_payload"); }
    } shm_remover;

    constexpr const std::size_t max_num_msg = 1;   // we need only 1 message for our synchronization
    constexpr const std::size_t max_msg_size = 0;  // an empty message is enough for our synchronization
    message_queue mq(open_or_create, "payload_is_constructed", max_num_msg, max_msg_size);
    unsigned int priority = 0;
    std::size_t received_size = 0;
    std::cout << "CONSUMER> receiving on mq" << std::endl;
    mq.receive(nullptr, 0, received_size, priority);
    std::cout << "CONSUMER> received on mq" << std::endl;

    // here, we received the signal from the producer, so we are sure that payload is properly constructed :
    shared_memory_object shm_obj(open_only, "my_payload", read_write);
    mapped_region region(shm_obj, read_write);
    Payload* ptr = static_cast<Payload*>(region.get_address());
    Payload& payload = *ptr;

    named_mutex consumer_must_work_mutex(open_or_create, CONSUMER_MUST_WORK_MUTEX);
    named_condition consumer_must_work_cv(open_or_create, CONSUMER_MUST_WORK_CV);
    {
        std::cout << "CONSUMER> about to lock" << std::endl;
        scoped_lock<named_mutex> lock(consumer_must_work_mutex);
        std::cout << "CONSUMER> after lock" << std::endl;
        while (!payload.consumer_must_work_bool) {
            std::cout << "CONSUMER> waiting notification bc 'consumer_must_work_bool' is " << std::boolalpha
                      << payload.consumer_must_work_bool << std::endl;
            consumer_must_work_cv.wait(lock);
            std::cout << "CONSUMER> wake up ! Maybe spurious ? For now, we don't know" << std::endl;
        }
        std::cout << "CONSUMER> notified ! (not a spurious one)" << std::endl;

        // on est débloqués ; on lit le message (pendant ce temps, l'autre thread se contente de lire) :
        char received_char = payload.shared_data;
        if (received_char == '') {
            std::cout << "=== End of reception, CONSUMER will now close ===" << std::endl;
        } else {
            std::cout << "CONSUMER got new data : '" << payload.shared_data << "'" << std::endl;
        }
    }

    std::cout << "CONSUMER> quitting here" << std::endl;

    // TODO = il faudra mieux gérer la destruction. Pour le moment je peux me contenter d'ici :
    ptr->~Payload();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    using namespace boost::interprocess;

    // C'est le seul truc que le parent a besoin de faire = s'assurer qu'on n'utilise pas une queue déjà existante
    message_queue::remove("payload_is_constructed");
    named_mutex::remove("my_super_mutex");
    if (fork() == 0) {
        // CHILD 1 :
        consumer();
    } else {
        producer();
    }
}

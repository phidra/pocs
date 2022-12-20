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
CE QUE MONTRE CETTE POC = équivalent de la POC 04 mais en utilisant boost::interprocess

Le principe reste d'avoir un producer (qui écrit des lettres sur la shared-mem) et un consumer (qui lit les lettres).

Ce qui change :
    pas de process parent qui construit/détruit le payload
        remplacé par le fait que le producer soit en charge de construire le payload...
        ... et le consumer attende que le producer ait construit le payload
        la synchro entre les deux se fait via une message-queue boost::interprocess
    utilisation d'un boost::interprocess::shared_memory_object comme mémoire partagée (plutôt que shm_open/mmap)
    utilisation de deux triplets {condition-variable + mutex + bool} pour synchroniser le producer et le consumer (plutôt que des attentes actives sur un atomic-bool)

Fonctionnement de la POC au final :
    En préambule, le process "parent" (i.e. avant fork) fait le ménage en appelant 'remove' sur toutes les structures interprocess.
    On forke en un couple {producer + consumer}
    Le consumer attend le signal du producer (message sur une message-queue boost::interprocess) avant de commencer à travailler.
    La shared-mem contenant le payload est create_only côté producer (d'où la précondition sur le fait qu'elle n'existe pas + les remove en préambule)
    La shared-mem contenant le payload est open_only côté consumer (d'où l'utilisation d'une MQ pour prévenir le consumer que la shared-mem est crée + le payload y a été construit)
    Le paylod est construit (appel au constructeur) côté producer, et détruit (appel au destructeur) côté consumer.
    Une fois le payload construit, le producer y publie les lettres une par une, et le consumer les lit.
    Le consumer attend d'être notifié par le producer par une CV (ou plus exactement un triplet {CV+mutex+bool})
    Une fois une lettre publiée, le producer attend d'être notifié par le consumer avec une autre CV (un autre triplet)
    Quand le producer n'a plus rien à publier, il envoie le caractère '' (= EOF) puis le process quitte.
    Quand le consumer reçoit le caractère '', il cleane tout (destructeur du payload + remove des structures interprocess), puis quitte.

À noter que ça n'est PAS la façon intelligente de faire communiquer un producer et un consumer (la bonne façon est une queue)...
... mais cette POC reste très utile pour me familiariser avec boost::ipc et les problématiques de communication interprocess.


J'ai lutté pour tout faire fonctionner, et j'ai appris plein de trucs :
    c'est *très* facile de faire foirer le système (notable exemple = je n'avais pas 'remove' en début de programme, or certains named-mutex avaient survécu, lockés)
    quand quelque chose se passe mal, c'est pas toujours reproductible, et pas toujours débuggable facilement
    avant d'utiliser un CV pour synchroniser deux process, ils doivent partager un flag commun
        on a donc une obligation de synchro pour partager ce flag commun avec les process AVANT de pouvoir utiliser des CV
    pour qu'un process appelle "nofity" sur une CV, il est inutile qu'il attende que l'autre process ait appelé "wait"
    les structures boost::interprocess existent dans /dev/shm (sur mon poste Linux) :
        la shared_memory, mais également le mutex (sous forme de sémaphore), et même les CV !
        seule la message-queue est gérée ailleurs, on peut la trouver sous /dev/mqueue, après sudo mount -t mqueue none /dev/mqueue
    les structures interprocess (named-mutex, notamment) survivent aux crash et autres terminaisons de process !
        pour repartir d'un état propre, on a donc une obligation de remove en début de POC
        identifier le "début de POC" nécessite de la synchro (e.g. pour éviter de remove la queue sur laquelle le producer vient de signaler qu'il avait construit le payload)
        dans cette POC, j'ai choisi que le process "parent" (i.e. le process juste avant le fork) fasse les remove préliminaires
    la question de qui supprime les ressources inter-process est importante, et quand :
        (je parle de la MQ, de la destruction du payload, de la libération des shared-mem, mutex, CV, etc.)
        d'une façon générale, si on cherche à cleaner les structures interprocess (pour laisser un état propre en s'éteignant), la question touchy est "quand s'assurer que plus personne n'utilise les structures"
        dans le cadre de ma POC, j'ai la chance de pouvoir les supprimer facilement en début de POC
        de plus, je m'arrange pour que le dernier à bosser soit le consumer, du coup pas de question à me poser, le consumer peut tout cleaner
        mais dans un cas un peu plus complexe, quand cleaner sera sans doute un sujet chevelu
        (il y a sans doute des trucs smart à faire à base de registering de process, mais clairement c'est hors-scope de ces notes d'en discuter)
    sur ce sujet, j'ai appris un truc intéressant sur shm_unlink (qui sous-tend sans doute les shared-memory boost) :
        (tout ce qui suit confond volontairement la shared-memory et shm_unlink, car je suppose que c'est bien ce qu'utilise boost en sous-jacent)
        on peut remove une shared-memory alors que quelqu'un est encore en train de s'en servir !
        en fait on peut shm_unlink tôt, même si qqun s'en sert, cf. man shm_unlink
            The operation of shm_unlink() is analogous to unlink(2):
            it removes a shared memory object name, and, once all processes have unmapped the object, de-allocates and destroys the contents of the associated memory region.
            After a successful shm_unlink(), attempts to shm_open() an object  with  the  same name fail (unless O_CREAT was specified, in which case a new, distinct object is created)
        c'est bien *après que tous les process ont unmappé la shared-memory* qu'elle est effectivement supprimée
        Du coup en réalité : on peut unlink autant que souhaité : la shared-memory restera utilisable par les process qui l'avaient déjà mmappée
        (par contre, dès le shm_unlink, on ne peut plus mapper la shared-memory dans un NOUVEAU process)
        (dans mon cas, ça fonctionne bien, car les process échangent préalablement par CV : au moment du remove de la shared-memory par le consumer, on sait que le producer avait déjà mmappé la shared-memory)

Je prévois de faire une autre POC plus simple, utilisant moins de ressources interprocess (e.g. utiliser payload.shared_data comme flag des CV)


EDIT : notes vrac complémentaires sur les message-queues :
    (en préambule : il y a DEUX types de mqueue : les mqueues posix et les mqueues boost)

    https://www.boost.org/doc/libs/1_80_0/doc/html/boost/interprocess/message_queue_t.html
        ~message_queue_t();
            Destroys *this and indicates that the calling process is finished using the resource. All opened message queues are still valid after destruction. The destructor function will deallocate any system resources allocated by the system for use by this process for this resource. The resource can still be opened again calling the open constructor overload. To erase the message queue from the system use remove(). 
        static bool remove(const char * name);
            Removes the message queue from the system. Returns false on error. Never throws

    man mq_overview :
        On Linux, message queues are created in a virtual filesystem. (...) This filesystem can be mounted (by the superuser) using the following commands:
            mkdir /dev/mqueue
            mount -t mqueue none /dev/mqueue
    ça me permet de lister les queues POSIX, et même de les supprimer !

    sauf que les MQ de boost IPC n'utilisent pas les MQ posix, apparemment :
        https://stackoverflow.com/a/63305924

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

#include <atomic>
#include <iostream>
#include <cstring>
#include <mqueue.h>
#include <chrono>
#include <thread>
#include <dirent.h>

#include "shared_mem.h"
#include "utils.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = c'est un dérivé de la POC 2, mais en construisant l'objet dans la shared-mem avec placement-new.
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
        les sous-process signalent qu'ils en ont fini avec le Payload avec une message-queue POSIX
    (c'est la POC qui forke les sous-process, inutile de lancer le binaire plusieurs fois)

cf. les commentaires détaillés avec des réponses à mes questions dans le code.

Par rapport à la POC2, au lieu de se reposer sur le caractère trivialement copyiable du Payload, on construit
explicitement celui-ci dans la shared-mem. Du coup, elle fonctionne avec n'importe quel Payload (alors que la deuxième
était réservée aux Payload trivialement coopyiable)

Avec la construction/destruction d'un objet dans la shared-mem, apparaissent des questions importantes :
- quand et par qui est créée la shared-mem ?
- comment s'assurer que la shared-mem est crée et initialisée (le Payload est construit) au moment où on l'utilise ?
- quand et par qui est détruite la shared-mem ?
- comment s'assurer que le Payload n'est plus utilisée par personne au moment où son destructeur est appelé ?

Pour répondre à ces questions dans la présente POC, j'ai utilisé une mq POSIX :
    on n'a plus deux process mais trois :
        le consumer, forké
        le producer, forké
        le process parent, qui, une fois les deux autres forkés, attend leur signalements pour libérer les ressources
    AVANT de forker les process fils, on crée une shared-memory, et on crée également la MQ qui synchronisera
    AVANT de forker les process fils, on construit le Payload dans la shared-mem
        (il n'y a donc pas de risque que les sous-process utilisent la shared-mem avant que le Payload ne soit construit)
    au moment de forker le consumer et le producer, ils prennent la shared_mem et la MQ en paramètre
    après avoir terminé leur boulot, le consumer et le producer envoient chacun un message sur la MQ
    après avoir forké les consumer+producer, le process parent attends deux messages sur la MQ
    une fois que le parent a reçu les messages, c'est que les fils ont fini de bosser -> il libère la MQ et la shared-mem
        (il n'y a donc pas de risque que le parent détruise le Payload alors que les sous-process l'utilisent encore)

En réalité, l'utilisation d'une MQ est inutilement compliquée...
En effet, il est safe de mmapper la shared-memory avant le fork (elle sera partagée par les processus-fils)
Du coup, je peux construire la shared-memory et l'initialiser tranquillement (placement-new).
Derrière, le process parent peut s'endormir en attendant que les fils aient fini de bosser.
Et ce qui change : il pourra utiliser le Payload partagé pour savoir QUAND les fils auront fini de bosser !
Je vais donc faire une quatrième POC en ce sens, qui utilise le payload plutôt que des MQ pour la synchronisation.

)DELIM";
    std::cout << std::endl;
}

struct Payload {
    std::atomic_bool consumer_has_work_to_do;
    std::atomic_bool producer_has_work_to_do;
    char shared_data;

    // NDM = la différence avec la POC précédente = ce constructeur est appelé :
    Payload() : consumer_has_work_to_do{false}, producer_has_work_to_do{false} {
        std::cout << "CONSTRUCTOR CALLED !" << std::endl;
    }

    // Du coup, il faut aussi appeler le destructor !
    ~Payload() { std::cout << "DESTRUCTOR CALLED !" << std::endl; }
};

void receive_from_mq(mqd_t queue_descriptor) {
    // vérification des attributs de la queue ici :
    mq_attr queue_attributes_at_receive;
    if (mq_getattr(queue_descriptor, &queue_attributes_at_receive) == -1) {
        raise_with_strerrno("Unable to mq_getattr");
    }
    std::cout << "attribute at receive : mq_flags = " << queue_attributes_at_receive.mq_flags << std::endl;
    std::cout << "attribute at receive : mq_maxmsg = " << queue_attributes_at_receive.mq_maxmsg << std::endl;
    std::cout << "attribute at receive : mq_msgsize = " << queue_attributes_at_receive.mq_msgsize << std::endl;
    std::cout << "attribute at receive : mq_curmsgs = " << queue_attributes_at_receive.mq_curmsgs << std::endl;

    char received_signal[20];  // sans que je comprenne pourquoi, le msgsize au receive est à 10 (alors que j'avais créé
                               // la queue avec une taille de 1), donc il faut que je passe un buffer suffisamment
                               // grand...

    ssize_t received_len = mq_receive(queue_descriptor, received_signal, 20, nullptr);
    if (received_len == -1) {
        raise_with_strerrno("Unable to mq_receive");
    }
    std::cout << "PARENT RECEIVED A SIGNAL OF SIZE " << received_len << " CONTAINING "
              << static_cast<int>(received_signal[0]) << std::endl;
}

void send_to_mq_and_close(mqd_t queue_descriptor, char sent_signal) {
    if (mq_send(queue_descriptor, &sent_signal, 1, 0) != 0) {
        raise_with_strerrno("Unable to mq_send");
    }
    mq_close(queue_descriptor);
}

void producer(SharedMem<Payload> const& writer, mqd_t queue_descriptor) {
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
        while (!payload.producer_has_work_to_do.load()) {
        }
    }

    std::cout << "=== End of emission, PRODUCER will now close ===" << std::endl;

    // Si on arrive ici, le producer n'a plus rien à faire, il prévient le process parent :
    send_to_mq_and_close(queue_descriptor, 69);
}

void consumer(SharedMem<Payload> const& reader, mqd_t queue_descriptor) {
    Payload& payload = reader.get();

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

    // Si on arrive ici, le consumer n'a plus rien à faire, il prévient le process parent :
    send_to_mq_and_close(queue_descriptor, 42);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    std::string shared_memory_name = "/mysupersharedmemory";
    std::string mq_name = "/mysuperqueue";

    // NDM : du coup, pour cette POC, j'ai besoin de synchro, je gère ça manuellement avec une MQ :
    std::cout << "\n---\n--- STEP 1 = creating MQ" << std::endl;
    struct mq_attr queue_attributes;
    queue_attributes.mq_flags = 0;
    queue_attributes.mq_maxmsg = 10;
    queue_attributes.mq_msgsize = 1;  // messages of exactly 1 byte
    queue_attributes.mq_curmsgs = 0;
    mqd_t queue_descriptor = mq_open(mq_name.c_str(), O_RDWR | O_CREAT, 0644, &queue_attributes);

    if (queue_descriptor == static_cast<mqd_t>(-1)) {
        raise_with_strerrno("Unable to mq_open");
    }

    std::cout << "\n---\n--- STEP 2 = creating shared-memory (before fork -> all child processes will share the same "
                 "fd and the same mmapped region)"
              << std::endl;
    SharedMem<Payload> shmem{shared_memory_name};

    // Cette SharedMem<Payload> est crée dans le process parent AVANT de forker les deux fils.
    // La structure sera donc partagée entre les 3 process (1 parent et 2 fils) :
    //      le file_descriptor sera cloné
    //      le T* (créé avec mmap dans le constructeur) pointera vers la même zone mémoire mmappée.
    // Créer la shared-memory AVANT les forks est plus simple et évite d'avoir à synchroniser la création de la zone
    // mémoire, ou d'avoir à gérer deux création de la zone mémoire différentes : l'une (la première) qui appelle le
    // constructeur, et les autres qui ne l'appellent pas.
    //
    // QUESTION : est-ce un souci ? Ne devrait-on pas mmapper à chaque fois APRÈS le fork ?
    // RÉPONSE : non ! Pour le mmap, les 3 process auront accès à la même memory-frame, chacun via son virtual address
    // space. Dit autrement, les process fils peuvent utiliser en sécurité une zone mémoire mmappée après un fork.
    //
    // QUESTION : du coup, SharedMem va être détruit 3 fois (une fois par process), est-ce problématique ?
    // RÉPONSE : du point de vue de la mémoire/désallocation : pas problématique. En effet, si on laisse de côté le COW,
    // tout se passe comme si le fait de forker avait dupliqué les memory-frame, il est donc attendu qu'on les
    // désalloue. Du point de vue de l'appel au destructeur, il fait munmap + close(file_descriptor). Dans les deux cas,
    // il faut bien effectuer ces opérations à la fois dans le parent et les enfants : En réalité, la vraie question se
    // pose pour shm_unlink et l'appel au destructeur ~Payload, qui ne doivent être faits qu'une seule fois. Dans cette
    // POC, je contourne le problème en ne libérant ces ressources que si la SharedMem est owner, et en ne marquant QUE
    // l'instance de SharedMem du process-parent comme owner -> ces fonctions ne seront bien appelées qu'une fois, comme
    // désiré.
    //
    //
    // QUESTION : du coup, sur le même mécanisme, les process se partagent-ils la même queue ?
    // RÉPONSE : oui ! On peut bien utiliser le même descriptor dans le parent et le child (et il faut bien le close
    // plusieurs fois, une fois dans chaque process).

    // NOTE : on pourrait tout à fait unlinker alors que la mémoire est en cours d'utilisation par un ou plusieurs
    // process.
    //        Le fichier n'existera plus, mais les memory-frame (partagées), si !
    //        Du coup même si le process forké n'a pas fini de bosser, c'est safe d'unlinker ici
    //        (à condition que le process forké ait commencé à utiliser la shared-mem, mais c'est bien le cas ici, car
    //        on ne peut atteindre cette ligne de code que s'il a été triggé plusieurs fois).
    //        cf. https://stackoverflow.com/questions/65860154/behaviour-of-shm-unlink/65860605#65860605
    //
    // QUELQUES RÉFÉRENCES :
    // man fork :
    // At the time of fork() both memory spaces have the same content.
    // The child inherits copies of the parent's set of open file descriptors
    // Each file descriptor in the child refers to the same open file description (...) as the corresponding file
    // descriptor in the parent.
    // (...)
    // The  child  inherits  copies  of the parent's set of open message queue descriptors (see mq_overview(7)).
    // Each file descriptor in the child refers to the same open message queue description as the corresponding file
    // descriptor in the parent.
    //
    // https://unix.stackexchange.com/questions/91058/file-descriptor-and-fork/91061#91061
    // When a child is forked then it inherits parent's file descriptors, if child closes the file descriptor what will
    // happen ? It inherits a copy of the file descriptor. So closing the descriptor in the child will close it for the
    // child, but not the parent, and vice versa.
    //
    // https://unix.stackexchange.com/questions/687403/how-does-a-process-and-its-children-use-memory-in-case-of-mmap/687413#687413
    // On fork() the memory space of the parent process is cloned into the child process.
    // (...)
    // "Both memory spaces have the same content" includes memory allocated with mmap().
    // The memory mappings get cloned and mmap() or munmap() after the fork don't affect the other process anymore.

    // NDM : le process parent va forker deux fois, pour producer puis consumer.
    //       je synchronise tout le monde avec une MQ
    //       in fine, le process parent se contente de réceptionner les signaux pour cleaner les ressources.
    if (fork() == 0) {
        // CHILD 1 :
        std::cout << "\n---\n--- STEP 3 = lancement du CONSUMER dans un subprocess" << std::endl;
        consumer(shmem, queue_descriptor);
    } else {
        if (fork() == 0) {
            // CHILD 2 :
            std::cout << "\n---\n--- STEP 4 = lancement du PRODUCER dans un subprocess" << std::endl;
            producer(shmem, queue_descriptor);
        } else {
            // PARENT :
            std::cout << "\n---\n--- STEP 5 = le process PARENT attend les signaux des deux sous-process pour libérer "
                         "les ressources"
                      << std::endl;

            // On ne doit appeler le destructeur ~Payload (et shm_unlink) qu'une seule fois. Comment ?
            // Seule la SharedMem<PayLoad> du parent est owner, et elle fait ces opérations spéciales à sa destruction.
            shmem.set_owner();

            // Pour être bien sûr que plus personne ne se sert du Payload, on attend de recevoir les signaux des process
            // :
            receive_from_mq(queue_descriptor);
            receive_from_mq(queue_descriptor);

            // Juste histoire de mieux comprendre ce qui se passe, j'inspecte les répertoires /dev :
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            std::cout << "\n---\n--- STEP 6 = avant de libérer les ressources, insepction des répertoires pertinents "
                         "de /dev :"
                      << std::endl;
            std::cout << std::endl;
            list_directory("/dev/shm/");
            list_directory("/dev/mqueue/");

            std::cout << "\n---\n--- STEP 7 = libération des ressources" << std::endl;
            // À ce stade, le process parent détruit manuellement la MQ :
            mq_close(queue_descriptor);
            mq_unlink(mq_name.c_str());

            // Par ailleurs, comme on s'apprête à sortir du scope, le destructeur ~SharedMem va être appelé.
            // Comme cette SharedMem est owner, elle appellera (manuellement !) le destructeur ~Payload sur la
            // zone mémoire de la shared-memory.
            // De plus, elle appellera shm_unlink.
        }
    }
}

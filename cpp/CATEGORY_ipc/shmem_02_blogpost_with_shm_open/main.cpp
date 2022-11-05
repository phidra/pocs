#include <atomic>
#include <iostream>
#include <cstring>

#include "shared_mem.h"

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = un autre exemple (meilleur que le premier) de deux process qui communiquent par mémoire partagée.

Cette POC est un poil (vraiment pas beaucoup) adaptée de ce post :
    https://subscription.packtpub.com/book/iot-&-hardware/9781838821043/7/ch07lvl1sec58/using-atomic-variables-in-shared-memory

Elle contient plein de trucs intéressants :
    une structure template gérant la libération des ressources par RAII :
        close le file-descriptor (à faire par chaque process)
        l'appel à munmap (à faire par chaque process)
        l'appel à shm_unlink (à ne faire que dans un seul process, identifié comme 'owner')
    le templating est fait sur un payload quelconque
        c'est ce payload qui est effectivement partagé dans la shared-mem
        il peut être n'importe quoi, y compris des structures permettant la synchronisation entre les process
        (c'est le cas ici : le payload comporte une donnée partagée, et deux atomic_bool pour la synchronisation)

Le point que je trouve le plus discutable concerne la façon dont on utilise le payload :
    il n'est jamais construit ! (on peut le vérifier en ajoutant un constructeur à Payload : il ne sera jamais appelé)
    de plus, j'ai vérifié expérimentalement que la shared_mem était initialisée avec des zéros, sans doute grâce à ftruncate
    pour moi, pour que la POC fonctionne, on se repose sur deux choses :
        le Payload est trivialement copyiable (ce que j'ai vérifié), on peut donc utiliser la zone mémoire sans constructeur
        le Payload (un int + deux atomic_bool) est tel qu'un layout mémoire rempli de zéro correspond à un Payload valide
Ça marche, mais ce n'est pas très générique comme façon de faire ! (je vais tenter une POC dérivée qui utilise placement-new)


Ce que fait la POC :
    la shared-mem est faite avec shm_open (plutôt que open) donc pas de fichier sur disque
    il va y avoir deux process :
        un producer (qui écrit sur la shared-mem)
        un consumer (qui lit la shared-mem)
    les deux process se synchronisent avec deux flags :
        quand le producer écrit, le consumer est bloqué
        ça n'est que quand le producer a fini d'écrire qu'il débloque le consumer
        quand le consumer lit, le producer est bloqué
        ça n'est que quand le consumer a fini de lire qu'il débloque le producer
    (c'est la POC qui forke le deuxième process, inutile de lancer le binaire deux fois)


Reste une question : comment la synchronisation des atomic a-t-elle lieu, sachant que la structure n'occupe qu'un seul byte ?
    ma compréhension, c'est que comme l'atomic est lock-free, son implémenation utilise des instructions qui sont atomic au niveau hardware (e.g. compare_and_swap) pour garantir l'atomicité de l'atomic_bool
    du coup, pas besoin d'autre chose que du bool lui-même, et il est normal que sizeof(atomic_bool) fasse 1 !


)DELIM";
    std::cout << std::endl;
}

const char* kSharedMemPath = "/sample_point";

struct Payload {
    // NDM : les deux flags seront sans doute à false initialement (car ftruncate sette la shared-mem à zéro)
    std::atomic_bool consumer_has_work_to_do;
    std::atomic_bool producer_has_work_to_do;
    char shared_data;

    // NDM = même si on décommentait ce constructeur, il ne serait jamais appelé !
    // En effet, la zone mémoire partagée est directement castée en un Payload*, sans placement-new.
    // La POC fonctionne quand-même malgré cette absence de construction, sans doute car le Payload est
    // trivialement-copyiable cf.
    // https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/4178176#4178176
    //      the lifetime begins when storage for the object is occupied and finishes when that storage is released or
    //      reused.
    // (je le garde justement commenté, car sinon, la structure n'est plus vraiment trivialement copyiable)

    /* Payload() { */
    /*     std::cout << "CONSTRUCTOR CALLED !" << std::endl; */
    /*     std::exit(42);  // on ne passe jamais ici */
    /* } */
};

void producer() {
    SharedMem<Payload> writer(kSharedMemPath);
    Payload& payload = writer.get();

    // NDM : pour partager un atomic entre plusieurs processes, il doit être lock-free :
    if (!payload.consumer_has_work_to_do.is_lock_free()) {
        throw std::runtime_error("Flag is not lock-free");
    }

    // NDM : le producer est le premier à commencer à travailler (car sa wait-loop est en fin de traitement)
    char data_to_send[] = "SKYWALKER";  // EOF signals the end of transmission
    std::cout << "PRODUCER will send this message : " << data_to_send << "\n" << std::endl;
    for (size_t i = 0; i < strlen(data_to_send); i++) {
        payload.shared_data = data_to_send[i];        // NDM : le producer sette la valeur à partager vers le consumer.
        payload.consumer_has_work_to_do.store(true);  // NDM : le producer signale au consumer qu'il peut bosser

        // NDM : le producer se met en attente bloquante (en attendant que le consumer sette producer_has_work_to_do) :
        payload.producer_has_work_to_do.store(false);
        while (!payload.producer_has_work_to_do.load())
            ;
    }

    std::cout << "=== End of emission, PRODUCER will now close ===" << std::endl;
}

void consumer() {
    SharedMem<Payload> point_reader(kSharedMemPath, true);
    Payload& payload = point_reader.get();

    // NDM : pour partager un atomic entre plusieurs processes, il doit être lock-free :
    if (!payload.consumer_has_work_to_do.is_lock_free()) {
        throw std::runtime_error("Flag is not lock-free");
    }

    char received_char;

    do {
        // NDM : le consumer ne fait rien tant qu'il n'a pas été triggé par le producer (via consumer_has_work_to_do)
        while (!payload.consumer_has_work_to_do.load())
            ;
        payload.consumer_has_work_to_do.store(false);

        // NDM : quand c'est le cas, il réceptionne la donnée partagée :
        received_char = payload.shared_data;
        if (received_char == '') {
            std::cout << "=== End of reception, CONSUMER will now close ===" << std::endl;
        } else {
            std::cout << "CONSUMER got new data : " << payload.shared_data << std::endl;
        }

        // NDM : il peut alors débloquer le producer, via producer_has_work_to_do :
        payload.producer_has_work_to_do.store(true);
    } while (received_char != '');
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    std::cout << std::endl;
    std::cout << "=== Le Payload est-il trivialement copyiable ? " << std::boolalpha << std::is_pod<Payload>::value
              << '\n';
    std::cout << std::endl;

    if (fork()) {
        consumer();
    } else {
        producer();
    }
}

#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

void print_poc_description() {
    std::cout << R"DELIM(
CE QUE MONTRE CETTE POC = équivalent de la POC 06 en essayant de simplifier la POC pour utiliser moins de ressources interprocess :

Le principe reste d'avoir un producer (qui écrit des lettres sur la shared-mem) et un consumer (qui lit les lettres).

Ce qui change :
    la création de la shared-memory + construction du payload peut être faite indifféremment par le producer ou le consumer (le plus rapide des deux s'en occupera)
    l'utilisation un mutex s'assure que si on crée la shared-memory, alors on construit le payload dedans
    dit autrement : si la shared-memory est accessible, alors le payload a été correctement construit dedans
    pour détecter si la shared-memory est construite, on essaye de la créer en catchant l'éventuelle exception à sa création)
    (ça permet de supprimer le recours à une message-queue, et le fait de forcer à ce que ce soit le producer qui créée la shared-mem)
    producer et consumer utilisent la même CV pour synchroniser leurs actions
    ce qui joue le rôle de flag pour la CV, c'est le payload lui-même

La destruction des ressources IPC + du payload reste "simple", i.e. je contourne la question
    (en effet, je sais que le consumer est le dernier à les utiliser -> il peut se charger du nettoyage)

Au final, la POC est beaucoup plus simple, les seules structures interprocess nécessaires sont :
    la shared-memory (duh, l'objectif de la POC est de partager des trucs via shared-memory)
    la CV
    le mutex (qui sert à deux choses indépendantes : initialiser le couple {shared-memory+payload}, puis synchroniser la CV)


)DELIM";
    std::cout << std::endl;
}

namespace bipc = boost::interprocess;

static char const* MUTEX_NAME = "shared_mutex";
static char const* CV_NAME = "shared_cv";
static char const* PAYLOAD_NAME = "my_payload";

struct Payload {
    char shared_data;

    Payload() :
        // le Payload commence avec un état déterminé = le shared_data est un caractère nul
        shared_data{'\0'} {
        std::cout << "PAYLOAD CONSTRUCTOR CALLED !" << std::endl;
    }

    ~Payload() { std::cout << "PAYLOAD DESTRUCTOR CALLED !" << std::endl; }
};

std::tuple<Payload*, bipc::mapped_region> get_or_create_payload(std::string process_name) {
    // NOTE : je renvoie également la mapped_region pour la conserver vivante.
    // en effet, je suppose qu'à la destruction de la mapped_region, la shared-memory peut être unmappée du VAS du
    // process courant.
    bipc::named_mutex the_mutex(bipc::open_or_create, MUTEX_NAME);
    bipc::scoped_lock<bipc::named_mutex> lock(the_mutex);

    try {
        bipc::shared_memory_object shm_obj(bipc::create_only, PAYLOAD_NAME, bipc::read_write);
        std::cout << "SHARED-MEM+PAYLOAD are created by '" << process_name << "'" << std::endl;
        // if we get here, creation of shared-memory has succeeded
        // this means that the current process was the first to acquire the creation mutex
        // we can construct the payload in the shared-memory
        // (for simplifcity, we assumer that the following lines can't throw interprocess_exception)
        shm_obj.truncate(sizeof(Payload));
        bipc::mapped_region region(shm_obj, bipc::read_write);
        Payload* ptr = new (region.get_address()) Payload{};
        return {ptr, std::move(region)};
    } catch (bipc::interprocess_exception& e) {
        std::cout << "SHARED-MEM+PAYLOAD are merely used (but not created) by '" << process_name << "'" << std::endl;
        // if we get here, creation of shared-memory has failed : the shared-memory was already created
        // this means that the Payload is alread constructed
        bipc::shared_memory_object shm_obj(bipc::open_only, PAYLOAD_NAME, bipc::read_write);
        bipc::mapped_region region(shm_obj, bipc::read_write);
        // pas de placement-new côté consumer : le Payload a DÉJÀ été construit par le producer :
        Payload* ptr = static_cast<Payload*>(region.get_address());
        return {ptr, std::move(region)};
    }
}

void producer() {
    auto result = get_or_create_payload("producer");
    Payload& payload = *(std::get<0>(result));

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
    auto result = get_or_create_payload("consumer");
    Payload* payload_ptr = std::get<0>(result);
    Payload& payload = *payload_ptr;

    // en revanche, on détruit le Payload côté consumer (qui est le dernier à s'en servir) :
    struct payload_destruction {
        payload_destruction(Payload* ptr_) : ptr{ptr_} {}
        ~payload_destruction() { ptr->~Payload(); }
        Payload* ptr;
    } payload_destructor{payload_ptr};

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
    bipc::shared_memory_object::remove(PAYLOAD_NAME);
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
        bipc::shared_memory_object::remove(PAYLOAD_NAME);
        bipc::named_mutex::remove(MUTEX_NAME);
        bipc::named_condition::remove(CV_NAME);
    } else {  // this is the child process from the fork (but this is irrelevant here)
        // décommenter cette ligne pour vérifier que tout est OK même si le producer démarre après le consumer :
        /* std::this_thread::sleep_for(2s); */

        producer();
    }
}

#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = l'utilisation de promises et futures pour déléguer du travail à un thread détaché :
    - on a un service MyService qui exposer une API pour créer du travail au long cours (sendMsg1 / sendMsg2)
    - cette API renvoie à l'appelant des std::future<int/string>
    - l'appelant peut utiliser les futures pour attendre de récupérer le travail au long cours

Le thread principal lance 3 tâches, récupère 3 futures, qui seront resolve à différents moments dans threads.
Grâce aux futures récupérées, il peut savoir quand le travail est terminé (voire se mettre en attente bloquante).


FONCTIONNEMENT DES PROMISES ET FUTURES :
========================================

En résumé, un couple future+promise représente un travail à réaliser, avec deux bouts de lorgnette sur ce travail :
- le client conserve la future (= un handle permettant d'attendre un résultat et savoir quand il est prêt)
- le worker conserve la promise (= un handle permettant de setter le résultat de la future correspondante)

Lorsque le worker a fini son travail, il sette le résultat sur la promise, ce qui débloque automatiquement la future correspondante.

ATTENTION : c'est à réserver aux cas one-shot ! (une promise ne peut se faire setter son résultat qu'une unique fois)

NOTE : ni les futures ni les promises ne peuvent être copiées (uniquement moved) :
    - https://en.cppreference.com/w/cpp/thread/future/future.html
    - https://en.cppreference.com/w/cpp/thread/promise/promise.html

NOTE : les futures doivent être VALIDES ; en pratique c'est toujours le cas, les seules futures non-valides sont :
    - les futures qu'on vient de moved (en effet, l'ancienne future ne contient plus rien)
    - les futures qui ont été default-constructed (au lieu d'être récupérées de façon normale = depuis une std::promise)

NOTE : lorsqu'on interroge l'état d'une future (via wait_XXX avec un timeout nul), elle peut avoir trois états :
    - ready = elle a reçu son résultat
    - timeout = elle n'a pas encore reçu son résultat
    - deferred = le travail n'a pas encore été exécuté, on attend d'en avoir besoin pour l'exécuter

NOTE : plusieurs sources me conduisent à dire qu'une fois qu'une promise a setté sa value, elle peut-être détruite
       sans risque, peu importe que la future ait déjà acquis son résultat ou pas :
           https://en.cppreference.com/w/cpp/thread/promise.html
           https://en.cppreference.com/w/cpp/thread/promise/~promise.html
           https://cplusplus.com/reference/future/promise/~promise/
               > Note that if other (future) objects are associated to the same shared state,
               > the shared state is itself not destroyed (until all those objects also release it).
       En effet, une fois l'état setté, il se comporte comme un état partagé avec compteur de référence.
       En revanche, si la promise est détruite SANS AVOIR SETTÉ SA VALEUR, les futures verront une exception "broken_promise".


)DELIM";
    cout << endl;
}

struct MyService {
    // quand on envoie le msg1, on reçoit un int en réponse :
    std::future<int> sendMsg1() {
        std::promise<int> response_promise;
        std::future<int> response_future = response_promise.get_future();
        {
            std::scoped_lock lock{the_mutex};
            promises_for_msg1.push_back(std::move(response_promise));
        }
        return response_future;
    }

    // quand on envoie le msg2, on reçoit une std::string en réponse :
    std::future<std::string> sendMsg2() {
        std::promise<std::string> response_promise;
        std::future<std::string> response_future = response_promise.get_future();
        {
            std::scoped_lock lock{the_mutex};
            promises_for_msg2.push_back(std::move(response_promise));
        }
        return response_future;
    }

    void resolve_msg1() {
        {
            std::scoped_lock lock{the_mutex};
            for (auto& prom : promises_for_msg1) {
                prom.set_value(42);
            }
            // NOTE : une fois `set_value` appelé, on peut librement détruire les promises,ça n'empêche pas les
            // futures associées d'accéder au résultat partagé (qui fonctionne avec un compteur de références).
            promises_for_msg1.clear();
        }
    }
    void resolve_msg2() {
        {
            std::scoped_lock lock{the_mutex};
            for (auto& prom : promises_for_msg2) {
                prom.set_value("POUET");
            }
            promises_for_msg2.clear();
        }
    }

    // Le service stocke les promises à débloquer
    std::mutex the_mutex;
    std::vector<std::promise<int>> promises_for_msg1;
    std::vector<std::promise<std::string>> promises_for_msg2;
};

template <typename ResultType>
bool has_result(std::future<ResultType> const& my_future) {
    // apparemment, la façon standard de vérifier si une future a reçu son résultat est de la wait avec un timeout nul :
    // https://stackoverflow.com/questions/10890242/get-the-status-of-a-stdfuture/10917945#10917945
    //
    auto status = my_future.wait_until(std::chrono::steady_clock::now());
    if (status == std::future_status::ready) {
        return true;
    }
    // les autres status possible sont :
    //      - timeout = la future n'a pas encore reçu son résultat
    //      - deferred = la future ne sera évaluée que lorsqu'on essayera de récupérer son résultat
    return false;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    MyService my_service;

    // STEP 1 = on créée du travail dans le service :
    std::future<int> future_int = my_service.sendMsg1();
    std::future<std::string> future_string = my_service.sendMsg2();
    std::future<std::string> future_string_bis = my_service.sendMsg2();
    auto display_futures = [&]() {
        cout << "    - future<int>     a son résultat = " << std::boolalpha << has_result(future_int) << endl;
        cout << "    - future<string>  a son résultat = " << std::boolalpha << has_result(future_string) << endl;
        cout << "    - future<string>2 a son résultat = " << std::boolalpha << has_result(future_string_bis) << endl;
    };

    // STEP 2 = dans un thread détaché, on effectue le travail (de type INT uniquement) après 2 secondes d'attente :
    std::thread t1{[&my_service]() {
        cout << "lancement du thread détaché, il attend 2 secondes avant de resolve la promise INT" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "le thread détaché s'apprête à resolve la promise INT !" << endl;
        my_service.resolve_msg1();
        cout << "le thread détaché vient de resolve la promise INT !" << endl;
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // à ce stade, aucun travail n'a encore été réalisé par le service (car les 2 secondes ne sont pas écoulées) :
    cout << "avant de resolve, quel est l'état des futures :" << endl;
    display_futures();
    cout << "on s'endort en attendant que le thread détaché termine de s'exécuter..." << endl;
    t1.join();

    // STEP 3 = le thread détaché a terminé, il a donc normalement effectué un travail, ce qui a dû modifier les futures
    // INT :
    cout << "le thread détaché vient de se terminer, quel est l'état des futures ?" << endl;
    display_futures();

    // STEP 4 = de même, dans un thread détaché, on s'endort 2 secondes, puis on effectue le travail des promises STRING
    // :
    std::thread t2{[&my_service]() {
        cout << "lancement du thread détaché, il attend 2 secondes avant de resolve la promise STRING" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "le thread détaché s'apprête à resolve la promise STRING !" << endl;
        my_service.resolve_msg2();
        cout << "le thread détaché vient de resolve la promise STRING !" << endl;

        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            cout << "le thread détaché n'a pas encore terminé (il peut continuer autre chose), ça n'a pas empêché de "
                    "débloquer les promise STRING !"
                 << endl;
        }
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // STEP 5 = mais ici, au lieu d'attendre que le thread ait terminé, on attend "juste" d'avoir le résultat pour les
    // futures STRING :
    cout << "on attend maintenant que la future STRING soit débloquée... (avec un timeout de 99s)" << endl;
    future_string.wait_for(std::chrono::seconds(99));
    future_string_bis.wait_for(std::chrono::seconds(99));
    cout << "la future STRING vient d'être débloquée, quel est l'état des futures ?" << endl;
    display_futures();

    t2.join();
    cout << "le thread détaché est terminé. Obviously, ça n'a pas modifié l'état des futures :" << endl;
    display_futures();
    return 0;
}

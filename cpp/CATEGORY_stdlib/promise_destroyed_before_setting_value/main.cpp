#include <future>
#include <iostream>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = le comportement des futures lorsque leur promise est détruite avant de setter un résultat.

NOTAMMENT :
    - lorsque la promise est détruite sans avoir setté de résultat, la future "gagne" aussitôt un résultat !
      (i.e. elle sort de l'attente bloquante)
    - le fait d'essayer d'accéder à ce résutlat via future.get() lève une exception
    - il y a plusieurs exceptions possibles, pour être certain qu'on a celle de la destruction trop tôtive de la promise,
      on peut analyser le code de l'exception...


Utiliser les broken-promises peut donc être un moyen de véhiculer un cas d'erreur côté worker-thread, où si le worker
thread rencontre un souci, il peut se contenter d'annuler la promise (l'alternative étant que la promise renvoie non
pas obligatoirement un résultat, mais un variant<résutlat, error>).



)DELIM";
    cout << endl;
}

struct MyService {
    // la fonction begin_work permet de déclencher un travail long à réaliser (matérialisé par la future en retour)
    std::future<int> begin_work() {
        std::promise<int> response_promise;
        std::future<int> response_future = response_promise.get_future();
        {
            std::scoped_lock lock{the_mutex};
            pending_promises.push_back(std::move(response_promise));
        }
        return response_future;
    }

    void finish_work() {
        {
            std::scoped_lock lock{the_mutex};
            for (auto& prom : pending_promises) {
                prom.set_value(42);
            }
            pending_promises.clear();
        }
    }

    // annuler une promise en cours :
    template <typename Duration>
    void destroy_all_pending_promises_after_delay(Duration delay) {
        std::this_thread::sleep_for(delay);
        {
            std::scoped_lock lock{the_mutex};
            pending_promises.clear();
        }
    }

    // Le service stocke les promises à débloquer
    std::mutex the_mutex;
    std::vector<std::promise<int>> pending_promises;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    MyService my_service;

    // =====
    cout << "Création d'un travail, ce qui renvoie 'my_future_1'" << endl;
    std::future<int> my_future_1 = my_service.begin_work();

    // =====
    cout << "Dans un thread détaché, on attend 2s puis on finish_work sur ce travail." << endl;
    std::thread t1{[&my_service]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        my_service.finish_work();
    }};
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // =====
    cout << endl;
    cout << "my_future_1 commence à attendre le résultat du travail..." << endl;
    my_future_1.wait_for(std::chrono::seconds(99));
    cout << "my_future_1 a fini d'attendre, elle a récupéré un résultat !" << endl;
    cout << "le résultat est = " << my_future_1.get() << endl;
    cout << endl;

    // =====

    cout << "On relance du travail, mais ce coup-ci, on va détruire la promise avant que le travail soit fini" << endl;
    std::future<int> my_future_2 = my_service.begin_work();
    std::thread t2{[&my_service]() { my_service.destroy_all_pending_promises_after_delay(std::chrono::seconds(1)); }};

    cout << "my_future_2 commence à attendre..." << endl;
    my_future_2.wait_for(std::chrono::seconds(99));
    cout << "my_future_2 a fini d'attendre, elle a récupéré un résultat !" << endl;
    cout << endl;

    try {
        cout << "le résultat est = " << my_future_2.get() << endl;
    } catch (std::future_error& e) {
        cout << "EXCEPTION !" << endl;
        cout << "WHAT = " << e.what() << endl;
        cout << "CODE = " << e.code() << endl;
        cout << std::boolalpha;
        cout << "Est-ce une broken-promise ? " << (e.code() == std::future_errc::broken_promise) << endl;
    }

    t1.join();
    t2.join();
    return 0;
}

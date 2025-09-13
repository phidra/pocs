#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using std::cout;
using std::endl;

void print_poc_description() {
    cout << R"DELIM(
CE QUE MONTRE CETTE POC = une "bonne" façon d'utiliser des condition-variables, notamment pour éviter les soucis liés :
    - aux spurious wakeups = où la cv a été réveillée à tort
    - aux lost wakeups = la cv a "raté" une notification


EN RÉSUMÉ :
    - la CV s'utilise avec un mutex, qu'il faut utiliser pour muter le predicate
    - attention, il faut utiliser la variante AVCE LE PREDICATE de wait_XXX (wait/wait_for/wait_until)
    - le predicate pose la question "dois-je me réveiller" :
        il doit passer à true quand a eu lieu l'evènement qu'attendant la CV pour se réveiller
    - utiliser les variantes avec predicate est suffisant pour qu'on n'ait ni spurious wakeups ni lost wakeups
    - si on doit synchroniser des états entre le thread réveillant et le thread dormant, il FAUT utiliser le mutex
        (on aura quoiqu'il arrive l'état du predicate à synchroniser entre les deux threads...)
        notamment, il ne faut PAS se contenter d'un atomic pour synchroniser les états !
    - ne pas oublier de release le mutex après le wait_XXX


cf. aussi [mes notes sur le sujet](https://github.com/phidra/notes/blob/main/structured/concurrency/condition_variables.md).


La situation illustrée par la POC ci-dessous :
    - on veut qu'un thread détaché fasse une tâche régulière
    - toutes les 200 ms, le thread se réveille, fait son travail, puis se rendort pour 200 ms
    - on veut également pouvoir interrompre à tout moment le thread depuis l'extérieur
    - on a donc besoin que le thread principal NOTIFIE le thread détaché quand il dort, d'où le besoin de CV...


FONCTIONNEMENT DE LA CV :
    cf. https://en.cppreference.com/w/cpp/thread/condition_variable.html :
        wait_XXX = atomically releases the mutex and suspends thread execution until the condition variable is notified,
        a timeout expires, or a spurious wakeup occurs, then atomically acquires the mutex before returning
    ^ tout est ici : la CV fait deux choses en même temps de façon atomique :
        1 = se mettre en attente bloquante d'écoute des notifications de réveil
        2 = libérer le mutex
    Derrière, lorsque wait_XXX retourne, le lock est réacquis par la CV
    Cette façon de faire garantit qu'on peut échanger des données entre un thread extérieur et le thread de la CV.
    Notamment, si le flag utilisé par le predicate est muté sous la protection du mutex, tout va bien :
        - avant de s'endormir, wait_XXX vérifie le predicate : s'il est déjà à true, elle ne s'endort même pas
            ^ pas de lost wake-ups possibles ✅
        - au moment où on endort la CV pour la première fois, on possède le lock :
        - le thread extérieur ne peut pas muter le flag entre le moment où wait_XXX vérifie le prédicat, et le moment où il s'endort
        - de façon atomique, la CV fait deux choses en même temps : release le mutex + s'endormir en écoutant les notifs
        - derrière, au moment de se réveiller (soit sur notification, soit à cause d'un spurious wakeup), la CV check le prédicat :
            s'il est faux, elle se rendort aussitôt → pas de spurious-wake-ups possibles ✅
            s'il est vrai, elle peut se réveiller pour de bon

GORY DETAILS = pourquoi a-t-on besoin du mutex ? Pourquoi ne pas se contenter d'un atomic pour gérer le flag du predicate ?
    Le mutex est la façon de fonctionner de la CV de toutes façons.
    Pire, cf. la doc https://en.cppreference.com/w/cpp/thread/condition_variable.html :
        Even if the shared variable is atomic, it must be modified while owning the mutex to
        correctly publish the modification to the waiting thread.
        ^ même si on utilise des atomics, il FAUT de toutes façons utiliser le mutex !
    Quoi qu'il arrive, on aura au moins un état à synchroniser entre les threads = le flag du pridcate.
    Du coup la bonne pratique est d'utiliser le mutex de la CV pour protéger les états partagés entre les threads.
    À NOTER = du côté du thread extérieur, on a besoin du mutex pour MUTER l'état du prédicat, mais pas pour NOTIFIER :
        Call notify_one (...) on the std::condition_variable (...) can be done after releasing the lock.

GORY DETAILS = pourquoi le mutex doit-il être released+wait de façon atomique ?
    Si on sépare les opérations, ça casse le fonctionnement des CV :
        écouter les notifications PUIS relâcher le mutex -> impossible, car dès qu'on écoute les notifications, on endort le thread
        relâcher le mutex PUIS écouter les notifications -> ça laisse une fenêtre où on peut notifier alors que la CV n'écoute pas encore
    IMPORTANT = quoi qu'il arrive, le thread de la CV doit release le mutex à la fin :
        déjà lorsque wait_XXX retourne, c'est que le thread a réacquis le mutex
        et de plus, on n'est même pas certain d'avoir vraiment dormi (donc d'avoir release le mutex) : si le predicate était déjà true

GORY DETAILS = pourquoi l'utilisation de wait_XXX avec predicate garantit qu'on n'a pas de lost wakeups ?
    En effet, j'ai d'abord pensé qu'il fallait essayer d'attendre que la CV soit correctement endormie avant de notifier...
    Mais avec les wait_XXX avec predicate, c'est inutile ; en effet, d'après la doc des versions avec predicate :
        https://en.cppreference.com/w/cpp/thread/condition_variable/wait_until
    D'après cppreference, la fonctions suivante :
        wait_until(lock, time_point, pred);
    Est équivalente à :
         while (!pred())
            if (wait_until(lock, abs_time) == std::cv_status::timeout)
                return pred();
        return true;
    Du coup si le prédicat est déjà vrai au moment du while, on n'endort même pas !
    Utiliser le predicate évite les lost wakeups non pas tant parce que la CV va recevoir la notification quand même...
        ... mais plutôt parce qu'elle n'en a pas besoin : elle ne s'endort même pas !
    Formulé autrement : ON A BIEN un lost wakeup ! (la CV rate bien réellement la notification !) Mais on s'en fiche :-)
    Formulé encore autrement : le fait de ne pas rater des notifications n'est qu'un détail d'implémentation...
    ...ce détail d'implémentation permet d'atteindre un objectif = réveiller le thread si un évènement s'est produit.
    Et c'est objectif est bien atteint, malgré la notification manquée, yay

)DELIM";
    cout << endl;
}

std::mutex the_mutex;
std::condition_variable cv;
bool should_wakeup = false;  // <------------ CECI EST LE FLAG DU PREDICATE !!!
std::chrono::milliseconds wait_period_ms{200};

void regular_worker() {
    // décommenter ceci si on veut montrer que ça se passe bien même si la notification a lieu AVANT de s'endormir :
    // std::this_thread::sleep_for(std::chrono::seconds{2});

    while (true) {
        std::unique_lock<std::mutex> lock(the_mutex);
        cout << endl;
        cout << "THREAD DORMANT> on s'apprête à s'endormir..." << endl;
        auto predicate = [] { return should_wakeup; };  // <----------- UTILISER LE PREDICATE EST TRÈS IMPORTANT !!!
        auto was_predicate_true_after_wait = cv.wait_for(lock, wait_period_ms, predicate);
        if (was_predicate_true_after_wait) {
            cout << "THREAD DORMANT> !!!!! CV réveillée par une notification externe (predicate=true) !!!!!" << endl;
            return;
        } else {
            cout << "THREAD DORMANT> CV réveillée car elle a timeouté : on 'fait le travail' puis on se rendort"
                 << endl;
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    // thread détaché = thread s'endormant régulièrement :
    std::thread detached_thread(regular_worker);

    // thread principal = après 1 seconde, arrêter le thread dormant :
    std::this_thread::sleep_for(std::chrono::seconds{1});
    {
        std::lock_guard<std::mutex> lock(the_mutex);
        should_wakeup = true;
    }
    cout << "THREAD EXTÉRIEUR> ------------------> on va réveiller le thread dormant................" << endl;
    cv.notify_one();

    // on n'oublie pas d'attendre le thread dormant avant de quitter :-)
    detached_thread.join();
    return 0;
}

#include <algorithm>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

int main(void) {
    const size_t nloop = 17;

    // version non-parallèle :
    {
        cout << "serial version :" << endl;
        for (int i = 0; i < nloop; i++) {
            {
                int const j = i * i;
                cout << j << endl;
            }
        }
        cout << endl;
    }

    // Version parallèle :
    const size_t nb_of_threads = 4;  // see also  std::thread::hardware_concurrency();
    {
        cout << "parallel version on " << nb_of_threads << " threads :" << endl;

        std::vector<std::thread> threads(nb_of_threads);
        std::mutex the_mutex;

        for (int thread_index = 0; thread_index < nb_of_threads; thread_index++) {
            auto worker_function = [&](int const loop_begin_for_this_thread, int const loop_end_for_this_thread) {
                // chaque thread va itérer uniquement sur une partie de l'input (la partie qui le concerne) :
                for (int i = loop_begin_for_this_thread; i < loop_end_for_this_thread; i++) {
                    const int j = i * i;
                    std::lock_guard<std::mutex> lock(the_mutex);
                    cout << j << endl;
                }
            };

            int const loop_begin_for_this_thread = thread_index * nloop / nb_of_threads;
            int const loop_end_for_this_thread =
                (thread_index + 1) == nb_of_threads ? nloop : (thread_index + 1) * nloop / nb_of_threads;
            auto worker_with_args = std::bind(worker_function, loop_begin_for_this_thread, loop_end_for_this_thread);

            // le thread est lancé dès qu'il est instancié sur un worker:
            threads[thread_index] = std::thread(worker_with_args);
        }

        // on attend que chaque thread se termine :
        std::for_each(threads.begin(), threads.end(), [](std::thread& x) { x.join(); });

        cout << endl;
    }
}

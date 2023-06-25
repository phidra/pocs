#include <iostream>
#include <vector>
#include <csignal>

// on déclenche l'envoi d'un signal SIGFPE en divisant par 0.
// un handler attrape le signal
// on montre qu'après retour du signal handler, la *MÊME* instruction est réexécutée !

using namespace std;

vector<int> v = {1, 2, 4, 0, 16};

unsigned int signal_counter = 0;

void my_handler(int sig)
{
    if (++signal_counter > 10)
    {
        exit(42);
    }
    cout << "SIGNAL #" << signal_counter << " : " << sig << endl;
}

int main(int argc, char* argv[])
{
    signal(SIGFPE, my_handler);

    for (const auto& x: v)
    {
        cout << " - " << (32.0 / x) << endl;
    }
    return 0;
}

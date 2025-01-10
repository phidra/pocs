#include <chrono>
#include <iostream>
#include <thread>

// NOTE : le main ne dépend publiquement ni de l'API ni du SDK OpenTelemetry
// Il utilise des librairies (app et init_telemetry) pour qui l'utilisation d'OpenTelemetry n'est
// pas publique, mais plutôt un détail d'implémentation.
#include "app.h"
#include "init_telemetry.h"

int main(void) {
    // dirty hack pour attendre que le container jaeger soit démarré :
    std::chrono::milliseconds sleep_duration{800};
    std::this_thread::sleep_for(sleep_duration);

    init_telemetry();
    std::cout << "PROCESSED = " << process_request("pouet") << std::endl;
    std::cout << "PROCESSED = " << process_request("pouet") << std::endl;
    std::cout << "PROCESSED = " << process_request("pouet") << std::endl;
    std::cout << std::endl;
    return 0;
}

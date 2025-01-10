#include <iostream>

// NOTE : le main ne dépend publiquement ni de l'API ni du SDK OpenTelemetry
// Il utilise des librairies (app et init_telemetry) pour qui l'utilisation d'OpenTelemetry n'est
// pas publique, mais plutôt un détail d'implémentation.
#include "app.h"
#include "init_telemetry.h"

int main(int argc, char* argv[]) {
    // dès qu'on passe un argument à la CLI, ça veut dire qu'on veut activer les vrais traces OTel :
    if (argc > 1) {
        std::cout << "INITIALIZING A REAL SDK TRACER" << std::endl;
        init_telemetry();
    } else {
        std::cout << "(using a noop tracer)" << std::endl;
    }
    std::cout << "PROCESSED = " << process_request("pouet") << std::endl;
    std::cout << std::endl;
    return 0;
}

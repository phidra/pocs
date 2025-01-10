#include <iostream>

// NOTE : le main ne dépend publiquement ni de l'API ni du SDK OpenTelemetry
// Il utilise des librairies (app et init_telemetry) pour qui l'utilisation d'OpenTelemetry n'est
// pas publique, mais plutôt un détail d'implémentation.
#include "app.h"
#include "init_telemetry.h"

int main(void) {
    init_telemetry();
    std::cout << "PROCESSED = " << process_request("pouet") << std::endl;
    std::cout << std::endl;
    return 0;
}

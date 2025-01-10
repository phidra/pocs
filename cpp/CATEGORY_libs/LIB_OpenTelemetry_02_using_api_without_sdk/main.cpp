#include <iostream>

// Ici, on n'utilise QUE l'API OpenTelemetry (et pas le SDK) :
#include "opentelemetry/trace/provider.h"

int main(void) {
    // en l'absence de SDK, on récupère un tracer DUMMY fourni par le singleton de l'API :
    auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("basic_tracer");

    // Du coup, ces appels sont sans effets :
    auto span = tracer->StartSpan("MySpan");
    span->AddEvent("MyEvent");
    span->End();

    std::cout << std::endl;
    std::cout << "Cette POC n'utilise que l'API header-only d'OpenTelemetry" << std::endl;
    std::cout << std::endl;

    return 0;
}

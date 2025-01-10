#include "app.h"

// Notre app utilise l'API OpenTelemetry pour instrumenter le code, mais n'utilise pas le SDK.
// (si OpenTelemetry n'a pas été initialisé, l'utilisation de cette API sera des noop)
// À noter que la dépendance de notre app à l'API OpenTelemetry n'est pas optionnelle (à la différence du SDK)
#include "opentelemetry/trace/provider.h"

std::string prefix(std::string const& in) {
    return std::string("[prefix+") + in;
}

std::string suffix(std::string const& in) {
    return std::string(in) + "+suffix]";
}

std::string process_request(std::string const& req) {
    // on ne sait pas ici si on récupère un vrai tracer ou un tracer dummy :
    auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("basic_tracer");

    auto span = tracer->StartSpan("MySpan");
    auto result = prefix(suffix(req));
    span->End();
    return result;
}

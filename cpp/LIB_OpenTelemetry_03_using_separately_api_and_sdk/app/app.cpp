#include "app.h"

// Ici, on n'utilise QUE l'API OpenTelemetry (et pas le SDK) :
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

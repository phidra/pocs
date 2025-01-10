#include "init_telemetry.h"

#include "opentelemetry/exporters/ostream/span_exporter.h"
#include "opentelemetry/trace/provider.h"

// La librairie 'init_telemetry' n'utilise le SDK OpenTelemetry *QUE* si on l'a configurée pour cela
// via le flag de compilation WITH_OPENTELEMETRY. Et dans tous les cas, c'est un détail d'implémentation.
#ifdef WITH_OPENTELEMETRY
#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
namespace sdktrace = opentelemetry::sdk::trace;
#endif  // WITH_OPENTELEMETRY

void init_telemetry() {
#ifdef WITH_OPENTELEMETRY
    std::cout << "INITIALIZING A REAL SDK TRACER" << std::endl;

    auto processor = std::unique_ptr<sdktrace::SpanProcessor>(new sdktrace::SimpleSpanProcessor(
        std::unique_ptr<sdktrace::SpanExporter>(new opentelemetry::exporter::trace::OStreamSpanExporter)));
    auto provider = opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
        new sdktrace::TracerProvider(std::move(processor)));
    opentelemetry::trace::Provider::SetTracerProvider(provider);
#else   // WITH_OPENTELEMETRY
    std::cout << "(using a noop tracer)" << std::endl;
#endif  // WITH_OPENTELEMETRY
}

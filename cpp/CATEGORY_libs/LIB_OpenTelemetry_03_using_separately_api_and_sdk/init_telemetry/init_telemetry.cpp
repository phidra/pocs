#include "init_telemetry.h"

// La librairie utilise le SDK OpenTelemetry (mais c'est un détail d'implémentation)
#include "opentelemetry/exporters/ostream/span_exporter.h"
#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/trace/provider.h"

namespace sdktrace = opentelemetry::sdk::trace;

void init_telemetry() {
    auto processor = std::unique_ptr<sdktrace::SpanProcessor>(new sdktrace::SimpleSpanProcessor(
        std::unique_ptr<sdktrace::SpanExporter>(new opentelemetry::exporter::trace::OStreamSpanExporter)));
    auto provider = opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
        new sdktrace::TracerProvider(std::move(processor)));
    opentelemetry::trace::Provider::SetTracerProvider(provider);
}

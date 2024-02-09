#include "init_telemetry.h"

#include <iostream>

#include "opentelemetry/trace/provider.h"

// La librairie 'init_telemetry' utilise le SDK OpenTelemetry ; c'est un détail d'implémentation.
#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
namespace sdktrace = opentelemetry::sdk::trace;

// cf. https://opentelemetry.io/docs/languages/cpp/exporters/#otlp-http-exporter
#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter_options.h"

namespace otlp = opentelemetry::exporter::otlp;

void init_telemetry() {
    std::cout << "INITIALIZING A REAL SDK TRACER THAT EXPORTS TO JAEGER" << std::endl;

    // export OTLP HTTP :
    otlp::OtlpHttpExporterOptions opts;
    opts.url = "http://localhost:4318/v1/traces";
    auto exporter = otlp::OtlpHttpExporterFactory::Create(opts);

    // Création du processeur de traces
    auto processor = std::unique_ptr<sdktrace::SpanProcessor>(new sdktrace::SimpleSpanProcessor(std::move(exporter)));

    auto resource = opentelemetry::sdk::resource::Resource::Create({{"service.name", "POC-OTel"}});

    auto provider = opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
        new sdktrace::TracerProvider(std::move(processor), resource));
    opentelemetry::trace::Provider::SetTracerProvider(provider);
}

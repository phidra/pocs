#include "init_telemetry.h"

#include "opentelemetry/trace/provider.h"
#include "opentelemetry/sdk/resource/resource.h"
#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_options.h"

namespace sdktrace = opentelemetry::sdk::trace;

void init_telemetry() {
    // exporter :
    opentelemetry::exporter::otlp::OtlpGrpcExporterOptions opts;
    opts.endpoint = "jaeger:4317";
    auto exporter = opentelemetry::exporter::otlp::OtlpGrpcExporterFactory::Create(opts);

    // processor :
    auto processor = std::unique_ptr<sdktrace::SpanProcessor>(new sdktrace::SimpleSpanProcessor(std::move(exporter)));

    // provider :
    auto resource_attributes = opentelemetry::sdk::resource::ResourceAttributes{{"service.name", "poc-opentelemetry"}};
    auto resource = opentelemetry::sdk::resource::Resource::Create(resource_attributes);
    auto provider = opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
        new sdktrace::TracerProvider(std::move(processor), resource));

    opentelemetry::trace::Provider::SetTracerProvider(provider);
}

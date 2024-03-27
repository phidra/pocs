#include <iostream>

#include "opentelemetry/exporters/ostream/span_exporter.h"
#include "opentelemetry/sdk/trace/simple_processor.h"
#include "opentelemetry/sdk/trace/tracer_provider.h"
#include "opentelemetry/trace/provider.h"

namespace sdktrace = opentelemetry::sdk::trace;

int main(void) {
    auto processor = std::unique_ptr<sdktrace::SpanProcessor>(new sdktrace::SimpleSpanProcessor(
        std::unique_ptr<sdktrace::SpanExporter>(new opentelemetry::exporter::trace::OStreamSpanExporter)));
    auto provider = opentelemetry::nostd::shared_ptr<opentelemetry::trace::TracerProvider>(
        new sdktrace::TracerProvider(std::move(processor)));
    opentelemetry::trace::Provider::SetTracerProvider(provider);
    auto tracer = provider->GetTracer("basic_tracer");
    auto span = tracer->StartSpan("MySpan");
    span->AddEvent("MyEvent");
    span->End();

    std::cout << "pouet" << std::endl;

    return 0;
}

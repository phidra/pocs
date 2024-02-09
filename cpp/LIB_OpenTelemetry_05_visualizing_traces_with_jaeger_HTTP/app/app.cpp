#include "app.h"

#include <chrono>
#include <thread>
#include <random>

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

constexpr const int lower_bound = 100;
constexpr const int upper_bound = 200;

struct RandomSleeper {
    RandomSleeper() : distribution(lower_bound, upper_bound) {}
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

    void sleep() {
        int randomNumber = distribution(generator);
        std::chrono::milliseconds duration(randomNumber);
        std::this_thread::sleep_for(duration);
    }
};

static RandomSleeper random_sleeper;

std::string process_request(std::string const& req) {
    auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("basic_tracer");

    // on enregistre une span de durée random :
    auto span = tracer->StartSpan("MySpan for " + req);
    random_sleeper.sleep();
    auto result = prefix(suffix(req));
    span->End();
    return result;
}

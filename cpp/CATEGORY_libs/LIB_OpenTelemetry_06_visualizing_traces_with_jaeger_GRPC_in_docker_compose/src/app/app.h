#pragma once

#include <string>

// NOTE : ce header public ne dépend pas d'OpenTelemetry
// (la dépendance de notre app à l'API OpenTelemetry est un détail d'implémentation)

std::string process_request(std::string const& req);

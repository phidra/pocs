#pragma once

#include <memory>

#include "Greeter.h"

enum class GreeterType { Simple, Reversed, Threaded };

std::unique_ptr<IGreeter> create_greeter(GreeterType);

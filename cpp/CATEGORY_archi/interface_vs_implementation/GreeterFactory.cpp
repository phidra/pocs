#include "GreeterFactory.h"

#include <stdexcept>
#include <iostream>

#include "GreeterImplSimple.h"
#include "GreeterImplThreaded.h"
#include "GreeterImplReversed.h"

std::unique_ptr<IGreeter> create_greeter(GreeterType greeter_type) {
    switch (greeter_type) {
        case GreeterType::Simple:
            std::cout << "About to simply greet :" << std::endl;
            return std::make_unique<GreeterImplSimple>();
        case GreeterType::Threaded:
            std::cout << "About to greet in MULTIPLE THREADS :" << std::endl;
            return std::make_unique<GreeterImplThreaded>();
        case GreeterType::Reversed:
            std::cout << "About to greet in REVERSE ORDER :" << std::endl;
            return std::make_unique<GreeterImplReversed>();
        default:
            throw std::runtime_error("unknown type");
    }
}

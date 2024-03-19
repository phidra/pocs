#pragma once

#include <string>
#include <memory>

// IMPORTANT : grâce à pimpl, myapp ne dépend plus publiquement de la lib !

struct MyApp {
    MyApp();
    ~MyApp();

    int get_age(std::string first_name, std::string last_name);

   private:
    struct PImpl;
    std::unique_ptr<PImpl> pimpl;
};

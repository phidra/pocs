#pragma once

#include <string>

struct Address {
    Address(int number, std::string street, std::string city);

    int number;
    std::string street;
    std::string city;
};

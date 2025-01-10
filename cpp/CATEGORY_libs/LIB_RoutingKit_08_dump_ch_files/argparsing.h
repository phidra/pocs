#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<unsigned> get_vector_u(std::string filename, std::string prog_name) {
    std::ifstream stream{filename};
    if (!stream.good()) {
        std::cerr << "ERROR: unable to read : '" << filename << "'\n";
        std::cerr << "\n";
        std::exit(2);
    }
    std::string line;
    int value;
    std::vector<unsigned> to_return;
    while (getline(stream, line).good()) {
        value = stoi(line);
        if (value < 0) {
            std::cerr << "ERROR : bad parsed value (" << value << ") from file : " << filename << std::endl;
            std::cerr << "\n";
            std::exit(3);
        }
        to_return.push_back(value);
    }
    return to_return;
}

std::vector<long> get_vector_l(std::string filename, std::string prog_name) {
    std::ifstream stream{filename};
    if (!stream.good()) {
        std::cerr << "ERROR: unable to read : '" << filename << "'\n";
        std::cerr << "\n";
        std::exit(2);
    }
    std::string line;
    long value;
    std::vector<long> to_return;
    while (getline(stream, line).good()) {
        value = stol(line);
        if (value < 0) {
            std::cerr << "ERROR : bad parsed value (" << value << ") from file : " << filename << std::endl;
            std::cerr << "\n";
            std::exit(3);
        }
        to_return.push_back(value);
    }
    return to_return;
}

int get_int(std::string int_arg, std::string prog_name) {
    int value = -1;
    try {
        value = stoi(int_arg);
    } catch (...) {
        std::cerr << "ERROR : unable to parse int value '" << int_arg << "'" << std::endl;
        std::cerr << "\n";
        std::exit(1);
    }
    return value;
}

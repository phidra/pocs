#ifndef ROUTING_KIT_VECTOR_IO_H
#define ROUTING_KIT_VECTOR_IO_H

#include <routingkit/bit_vector.h>

#include <fstream>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace RoutingKit {

template <class T>
void save_vector(std::string const& file_name, std::vector<T> const& vec) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    std::ofstream out(file_name, std::ios::binary);
    if (!out)
        throw std::runtime_error("Can not open \"" + file_name + "\" for writing.");
    out.write(reinterpret_cast<char const*>(&vec[0]), vec.size() * sizeof(T));
}

template <class T>
std::vector<T> load_vector(std::string const& file_name) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    std::ifstream in(file_name, std::ios::binary);
    if (!in)
        throw std::runtime_error("Can not open \"" + file_name + "\" for reading.");
    in.seekg(0, std::ios::end);
    unsigned long long file_size = in.tellg();
    if (file_size % sizeof(T) != 0)
        throw std::runtime_error("File \"" + file_name +
                                 "\" can not be a vector of the requested type because it's size is no multiple of the "
                                 "element type's size.");
    in.seekg(0, std::ios::beg);
    std::vector<T> vec(file_size / sizeof(T));
    in.read(reinterpret_cast<char*>(&vec[0]), file_size);
    return vec;  // NVRO
}

template <>
void save_vector<std::string>(std::string const& file_name, std::vector<std::string> const& vec);
extern template void save_vector<std::string>(std::string const& file_name, std::vector<std::string> const& vec);

template <>
std::vector<std::string> load_vector<std::string>(std::string const& file_name);
extern template std::vector<std::string> load_vector<std::string>(std::string const& file_name);

void save_bit_vector(std::string const& file_name, BitVector const& vec);
BitVector load_bit_vector(std::string const& file_name);

template <class T>
void save_value(std::string const& file_name, const T& val) {
    save_vector(file_name, std::vector<T>{val});
}

template <class T>
T load_value(std::string const& file_name) {
    auto v = load_vector<T>(file_name);
    if (v.empty())
        throw std::runtime_error(file_name + " is empty");
    if (v.size() > 1)
        throw std::runtime_error(file_name + " contains more than one element");
    return v.front();
}

template <class T>
void write_value(std::ostream& out, const T& val) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    out.write((char const*)&val, sizeof(T));
    if (!out)
        throw std::runtime_error("Could not write value to file");
}

template <class T>
T read_value(std::istream& in) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    T val;
    in.read((char*)&val, sizeof(T));
    if (!in)
        throw std::runtime_error("Could not read value to file");
    return val;  // NVRO
}

template <class T>
void write_vector(std::ostream& out, std::vector<T> const& v) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    out.write((char const*)&v[0], sizeof(T) * v.size());
    if (!out)
        throw std::runtime_error("Could not write vector data");
}

template <class T>
std::vector<T> read_vector(std::istream& in, unsigned long long size) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    std::vector<T> v(size);
    in.read((char*)&v[0], sizeof(T) * size);
    if (!in)
        throw std::runtime_error("Could not read vector data");
    return v;  // NVRO
}

template <class T>
void write_value(std::function<void(char const*, unsigned long long)> const& out, const T& val) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    out((char const*)&val, sizeof(T));
}

template <class T>
T read_value(std::function<void(char*, unsigned long long)> const& in) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    T val;
    in((char*)&val, sizeof(T));
    return val;  // NVRO
}

template <class T>
void write_vector(std::function<void(char const*, unsigned long long)> const& out, std::vector<T> const& v) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    out((char const*)&v[0], sizeof(T) * v.size());
}

template <class T>
std::vector<T> read_vector(std::function<void(char*, unsigned long long)> const& in, unsigned long long size) {
    static_assert(std::is_pod<T>::value,
                  "Cannot find non-trivial serialization code for this type, maybe a header is missing or "
                  "serialization is simply not available");
    std::vector<T> v(size);
    in((char*)&v[0], sizeof(T) * size);
    return v;  // NVRO
}

inline BitVector read_bit_vector(std::function<void(char*, unsigned long long)> const& in, unsigned long long size) {
    BitVector v(size);
    in((char*)v.data(), ((size + 511) / 512) * 64);
    return v;
}

inline void write_bit_vector(std::function<void(char const*, unsigned long long)> const& out, BitVector const& v) {
    out((char const*)v.data(), ((v.size() + 511) / 512) * 64);
}

template <class F>
void open_file_for_saving(std::string const& file_name, const F& f) {
    std::ofstream out(file_name, std::ios::binary);
    if (!out)
        throw std::runtime_error("Can not open \"" + file_name + "\" for writing.");
    f(out);
}

template <class F>
void open_file_for_loading(std::string const& file_name, const F& f) {
    std::ifstream in(file_name, std::ios::binary);
    if (!in)
        throw std::runtime_error("Can not open \"" + file_name + "\" for reading.");

    in.seekg(0, std::ios_base::end);
    unsigned long long file_size = in.tellg();
    in.seekg(0, std::ios_base::beg);
    f(in, file_size);
}

}  // namespace RoutingKit

#endif

#ifndef FILE_DATA_SOURCE_H
#define FILE_DATA_SOURCE_H

#include <functional>
#include <string>

#ifdef ROUTING_KIT_NO_POSIX
#include <stdio.h>
#endif

namespace RoutingKit {

class FileDataSource {
   public:
    FileDataSource();
    FileDataSource(char const* file_name);
    FileDataSource(std::string const& file_name);

    unsigned long long size() const;

    void rewind();

    void open(char const* file_name);
    void open(std::string const& file_name) { open(file_name.c_str()); }

    void close();

    FileDataSource(FileDataSource const&) = delete;
    FileDataSource const& operator=(FileDataSource const&) = delete;

    FileDataSource(FileDataSource&& o);
    FileDataSource const& operator=(FileDataSource&& o);

    unsigned long long minimum_read_size() const { return 1; }

    unsigned long long read(char* buffer, unsigned long long to_read);

    std::function<unsigned long long(char*, unsigned long long)> get_read_function_object();

    ~FileDataSource() { close(); }

   private:
#ifndef ROUTING_KIT_NO_POSIX
    int file_descriptor;
#else
    FILE* file_descriptor;
#endif
};

}  // namespace RoutingKit

#endif

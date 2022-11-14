#pragma once

#include <string>
#include <sstream>
#include <dirent.h>
#include <cstring>

inline void list_directory(std::string dirname) {
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(dirname.c_str())) != NULL) {
        /* print all the files and directories within directory */
        printf("Content of %s :\n", dirname.c_str());
        while ((ent = readdir(dir)) != NULL) {
            printf("    %s\n", ent->d_name);
        }
        printf("\n");
        closedir(dir);
    } else {
        std::ostringstream oss;
        oss << "Unable to open directory '" << dirname << "' : " << strerror(errno) << std::endl;
        throw std::runtime_error(oss.str());
    }
}

inline void raise_with_strerrno(std::string msg_prefix) {
    std::ostringstream oss;
    oss << msg_prefix << " : " << strerror(errno);
    throw std::runtime_error(oss.str());
}

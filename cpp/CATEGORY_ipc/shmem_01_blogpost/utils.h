#include <iostream>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>

inline size_t get_file_size(int fd) {
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
}

enum class LockType {
    EXCLUSIVE = LOCK_EX,
    SHARED = LOCK_SH,
    UNLOCK = LOCK_UN,
};

struct FileLock {
    int file_descriptor;
    LockType lock_type;
    useconds_t spin_duration;
    FileLock(int file_descriptor_, LockType lock_type_, useconds_t spin_duration_ = 10000)
        : file_descriptor{file_descriptor_}, lock_type{lock_type_}, spin_duration{spin_duration_} {
        // acquiring lock :
        while (flock(file_descriptor, static_cast<int>(lock_type)) != 0) {
            usleep(spin_duration);  // spinning  (WARNING : there is no timeout)
        }
    }
    ~FileLock() {
        std::cout << "Releasing lock on file_descriptor\n" << std::flush;
        flock(file_descriptor, static_cast<int>(LOCK_UN));
    }

    FileLock(FileLock const&) = delete;
    FileLock(FileLock&&) = delete;
    FileLock& operator=(FileLock const&) = delete;
    FileLock& operator=(FileLock&&) noexcept = delete;
};

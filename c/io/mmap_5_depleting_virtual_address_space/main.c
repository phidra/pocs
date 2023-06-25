#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/time.h>

#define MEG 1048576

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    int fd = open(argv[1], O_RDONLY, 0755);
    if (fd < 0) {
        printf("open failed !");
        return 10;
    }

    int mmapped_size = 100 * MEG;

    int nb_loop = 50;

    printf("--- About to mmap the same 100 Mio file %i times\n", nb_loop);
    sleep(0.5);
    unsigned int total_mmaped_size = 0;
    for (int counter = 0; counter < nb_loop; ++counter) {
        void* addr = mmap(0, mmapped_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (addr == MAP_FAILED) {
            printf("mmap failed : %s", strerror(errno));
            exit(1);
        }
        total_mmaped_size += mmapped_size;
        printf("--- [%i] just mmapped file, total mmapped = %u Mio\n", counter, total_mmaped_size / (1024 * 1024));
        sleep(0.2);
    }

    return 0;
}

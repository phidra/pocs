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

    int mmapped_size = 512 * MEG;

    char BUF[MEG];
    memset(BUF, '\0', MEG);
    printf("--- Juste allocated a 1 Mb buffer\n");

    sleep(1);
    printf("--- About to mmap file\n");
    sleep(1);
    void* addr = mmap(0, mmapped_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
        printf("mmap failed : %s", strerror(errno));
    printf("--- Just mmapped file of size : %i\n", mmapped_size);
    sleep(1);

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    printf("--- beginning a bunch of copies from mmapped file\n");
    int len = mmapped_size / MEG;
    for (int i = 0; i < len; ++i) {
        int offset = i * MEG;
        memcpy(BUF, addr + offset, MEG);
    }

    gettimeofday(&tv2, NULL);

    double time_taken = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("--- just ended the copy from mmapped file, took %f seconds\n", time_taken);
    printf("--- volume copied = %i Mb\n", len);

    int sleep_duration = 3;
    printf("--- Now sleeping %i seconds\n", sleep_duration);
    sleep(sleep_duration);
    printf("--- exiting\n");

    return 0;
}

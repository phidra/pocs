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
    int fd = open(argv[1], O_RDWR, 0755);
    if (fd < 0) {
        printf("open failed !");
        return 10;
    }

    int mmapped_size = 100 * MEG;

    char BUF[MEG];
    memset(BUF, '\0', MEG);

    printf("--- About to mmap file\n");
    sleep(1);
    void* addr = mmap(0, mmapped_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        printf("mmap failed : %s", strerror(errno));
        exit(1);
    }
    printf("--- Just mmapped file of size : %i\n", mmapped_size);
    sleep(1);

    struct timeval tv1, tv2;

    printf("--- beginning to loop while copying from mmapped file (with 1s sleep inbetween)\n");
    int len = mmapped_size / MEG;
    for (int counter = 0; counter < 10; ++counter) {
        if (counter == 5) {
            printf("WRITING !!!\n");
            for (int i = 0; i < len; ++i) {
                int offset = i * MEG;
                for (int j = 0; j < 255; ++j) {
                    for (int k = 0; k < 1024; ++k) {
                        *((char*)(addr + offset + (j * 1024 + k))) = 'M';
                    }
                }
            }
        }

        int val = 0;
        gettimeofday(&tv1, NULL);
        for (int i = 0; i < len; ++i) {
            int offset = i * MEG;
            memcpy(BUF, addr + offset, MEG);
            val += (int)BUF[0];
        }
        gettimeofday(&tv2, NULL);
        double time_taken = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
        printf("--- just ended the copy from mmapped file (%i), took %f seconds\n", val, time_taken);
        sleep(1);
    }

    return 0;
}

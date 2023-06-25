#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/time.h>

int main(int argc, char* argv[]) {
    int nb_of_loops = 20;
    assert(argc >= 2);
    int fd = open(argv[1], O_RDONLY, 0755);
    if (fd < 0) {
        printf("open failed !");
        return 10;
    }

    int bufsize = 1024;
    char destination[bufsize];
    printf("--- About to loop-reading file '%s' for %i times\n", argv[1], nb_of_loops);
    off_t lseek_returned = lseek(fd, 0, SEEK_SET);

    if (lseek_returned < 0) {
        printf("lseek failed !");
        return 20;
    }

    struct timeval tv1, tv2;

    for (int counter = 0; counter < nb_of_loops; ++counter) {
        gettimeofday(&tv1, NULL);
        ssize_t actually_read = read(fd, destination, bufsize);
        if (actually_read <= 0) {
            printf("read failed !");
            return 30;
        }
        gettimeofday(&tv2, NULL);
        double time_taken_s = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
        double time_taken_ms = time_taken_s * 1000;
        printf("--- just ended the read, took %f ms\n", time_taken_ms);
        sleep(1);
    }

    char displayed[64] = {0};
    memcpy(displayed, destination, 63);
    printf("FIRST BYTES OF READ CONTENT = %s", displayed);

    sleep(1);

    return 0;
}

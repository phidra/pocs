#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_LEN 1024
char BUF1[BUFFER_LEN];
char BUF2[BUFFER_LEN];

off_t BEGIN = 0;
off_t BEGIN_PLUS_8192 = 8192;

void do_seek(int fd, off_t pos) {
    off_t status = -1;
    status = lseek(fd, pos, SEEK_SET);
    if (status != pos) {
        printf("LSEEK TO %d ERROR : %s\n", (int)pos, strerror(errno));
        exit(1);
    }
}

void do_write(int fd, char* content, size_t length) {
    ssize_t written = write(fd, content, length);
    if (written == -1) {
        printf("WRITE %d BYTES ERROR : %s\n", (int)length, strerror(errno));
        exit(2);
    }
    assert(written == length);

    int status = fsync(fd);
    if (status != 0) {
        printf("FSYNC AFTER WRITE %d BYTES ERROR : %s\n", (int)length, strerror(errno));
        exit(3);
    }
}

int do_open(char* file) {
    int fd = open(file, O_RDWR);
    if (fd == -1) {
        printf("OPEN '%s' ERROR : %s\n", file, strerror(errno));
        exit(4);
    }
    return fd;
}

void do_close(int fd) {
    int status = close(fd);
    if (fd == -1) {
        printf("CLOSE ERROR : %s\n", strerror(errno));
        exit(5);
    }
}

void do_read(int fd, char* buffer, int length) {
    ssize_t got = read(fd, buffer, length);
    if (got == -1) {
        printf("READ %d BYTES ERROR : %s\n", length, strerror(errno));
        exit(6);
    }
    buffer[length] = 0;
}

int main(int argc, char* argv[]) {
    assert(argc >= 4);
    char* TEMPFILE = argv[1];
    int CHUNK_SIZE = atoi(argv[2]);
    int NB_LOOP = atoi(argv[3]);
    assert(CHUNK_SIZE > 500 && CHUNK_SIZE < 5000);
    assert(NB_LOOP > 0);
    assert(BUFFER_LEN > CHUNK_SIZE);  // if necessary, increase BUFFER_LEN so that this assert doesn't fail

    printf("\nAbout to swap chunks of %d bytes on file : '%s'.\n", CHUNK_SIZE, TEMPFILE);
    printf("Doing that for %d times...\n", NB_LOOP);

    int fd = do_open(TEMPFILE);

    memset(BUF1, 0, BUFFER_LEN);
    memset(BUF2, 0, BUFFER_LEN);

    for (int i = 0; i < NB_LOOP; ++i) {
        // swapping disc content at BEGIN and BEGIN_PLUS_8192
        do_seek(fd, BEGIN);
        do_read(fd, BUF1, CHUNK_SIZE);

        do_seek(fd, BEGIN_PLUS_8192);
        do_read(fd, BUF2, CHUNK_SIZE);

        do_seek(fd, BEGIN);
        do_write(fd, BUF2, CHUNK_SIZE);

        do_seek(fd, BEGIN_PLUS_8192);
        do_write(fd, BUF1, CHUNK_SIZE);
    }

    do_close(fd);

    return 0;
}

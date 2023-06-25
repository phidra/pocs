#define _GNU_SOURCE /* necessary for O_DIRECT */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    /* O_SYNC | O_DIRECt -> no page cache, but has alignment needs */
    int fd = open("mytestfile.GITIGNORE", O_WRONLY | O_CREAT /*| O_SYNC | O_DIRECT*/, 0755);
    if (fd < 0) {
        printf("ARF 1\n");
        return 10;
    }

    char* mystr = "Salut le monde !";
    ssize_t written = write(fd, mystr, strlen(mystr));

    if (written < 0) {
        printf("ARF 2\n");
        const char* pouet = strerror(errno);
        printf("%s\n", pouet);
        return 20;
    }

    int closed = close(fd);
    if (closed < 0) {
        printf("ARF 3\n");
        if (errno == EIO) {
            printf("EIO");
        } else if (errno == EBADF) {
            printf("EBADF");
        } else if (errno == EINTR) {
            printf("EINTR");
        } else {
            printf("unknown errno");
            const char* pouet = strerror(errno);
            printf("%s\n", pouet);
        }
        return 30;
    }

    printf("seems good...\n");
    return 0;
}

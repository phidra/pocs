#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char* MODIFICATION = "===[COUCOU]===";

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    char* TEMPFILE = argv[1];
    size_t MODIFICATION_LEN = strlen(MODIFICATION);

    struct stat buf;
    int stat_status = stat(TEMPFILE, &buf);

    printf("I will now modify the end of this file : %s\n", TEMPFILE);
    printf("Size of the file = %d bytes\n", (int)buf.st_size);

    int fd = open(TEMPFILE, O_WRONLY);
    if (fd < 0) {
        printf("ARF 1\n");
        return 10;
    }

    // 10 char will be left unchanged at the end of the file :
    off_t desired_position = -(11 + MODIFICATION_LEN);
    off_t lseek_rc = lseek(fd, desired_position, SEEK_END);
    if (lseek_rc == -1) {
        printf("ARF 2\n");
        return 20;
    }

    ssize_t written = write(fd, MODIFICATION, MODIFICATION_LEN);

    if (written < 0) {
        printf("ARF 3\n");
        return 30;
    }

    int closed = close(fd);
    if (closed < 0) {
        printf("ARF 4\n");
        return 40;
    }

    printf("ok for the file modification...\n");
    return 0;
}

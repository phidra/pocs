#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    assert(argc >= 3);
    char* TEMPFILE = argv[1];
    char* MODIFICATION = argv[2];
    size_t MODIFICATION_LEN = strlen(MODIFICATION);

    struct stat buf;
    int stat_status = stat(TEMPFILE, &buf);

    printf("I will now modify the end of this file : %s\n", TEMPFILE);
    printf("Size of the file = %d bytes\n", (int)buf.st_size);

    FILE* stream = fopen(TEMPFILE, "r+");
    if (!stream) {
        printf("ARF 1\n");
        return 10;
    }

    // 10 char will be left unchanged at the end of the file :
    long desired_position = -(11 + MODIFICATION_LEN);
    int fseek_rc = fseek(stream, desired_position, SEEK_END);
    if (fseek_rc == -1) {
        printf("ARF 2 : %d  -  %s\n", fseek_rc, strerror(errno));
        return 20;
    }

    ssize_t written = fwrite(MODIFICATION, MODIFICATION_LEN, 1, stream);
    if (!written) {
        printf("ARF 3\n");
        return 30;
    }
    fflush(stream);

    int closed = fclose(stream);
    if (closed) {
        printf("ARF 4\n");
        return 40;
    }

    printf("ok for the file modification...\n");
    return 0;
}

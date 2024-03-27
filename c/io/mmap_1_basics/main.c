#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    int fd = open(argv[1], O_RDONLY, 0755);
    if (fd < 0) {
        printf("open failed !");
        return 10;
    }

    char BUF[64];
    memset(BUF, '\0', 64);

    sleep(1);  // to retrieve this point in strace output
    void* addr = mmap(0, 16384, PROT_READ, MAP_PRIVATE, fd, 0);

    printf("---\n");
    for (int i = 0; i < 16; ++i) {
        // read 16 times 64 bytes (leave last byte to 0 for null-terminated string)
        memcpy(BUF, (addr + i * 1024), 63);
        printf("%s\n", BUF);
        printf("---\n");
    }

    return 0;
}

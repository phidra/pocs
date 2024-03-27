#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define MEG 1048576         // 1024 kio = 1 Mio
#define SLEEP_IN_S 1000000  // usleep uses microseconds, convert it in seconds

void display_proc_self_status() {
    // note : j'ai pu vérifier que les infos de /proc/self/statm et celles de /proc/self/status étaient cohérentes
    // (heureusement !)
    char* the_file = "/proc/self/status";
    int fd = open(the_file, O_RDONLY, 0755);
    if (fd < 0) {
        printf("open failed !\n");
        exit(11);
    }

    char buf[2048] = {0};
    int actually_read = read(fd, buf, 2048);
    if (actually_read <= 0) {
        printf("read failed !\n");
        exit(21);
    }

    /* printf("My own PID is %i\n", (int)getpid()); */
    /* printf("Raw '%s' is %s\n", the_file, buf); */

    char* token = strtok(buf, "\n");
    while (token) {
        if (memcmp(token, "VmSize", 6) == 0)
            printf("Virtual memory size   | %s\n", token);
        if (memcmp(token, "VmRSS", 5) == 0)
            printf("Resident set size     | %s\n", token);
        if (memcmp(token, "VmData", 6) == 0)
            printf("Size of data segment  | %s\n", token);
        if (memcmp(token, "VmStk", 5) == 0)
            printf("Size of stack segment | %s\n", token);
        if (memcmp(token, "VmExe", 5) == 0)
            printf("Size of text segment  | %s\n", token);
        if (memcmp(token, "VmLib", 5) == 0)
            printf("Shared lib code size  | %s\n", token);
        if (memcmp(token, "VmSwap", 6) == 0)
            printf("Swapped-out virt mem  | %s\n", token);
        if (memcmp(token, "RssAnon", 7) == 0)
            printf("Resident anonymous mem| %s\n", token);
        if (memcmp(token, "RssFile", 7) == 0)
            printf("Resident file mappings| %s\n", token);
        if (memcmp(token, "RssShmem", 8) == 0)
            printf("Resident shared mems  | %s\n", token);
        token = strtok(0, "\n");
    }

    int rc = close(fd);
    if (rc != 0) {
        printf("close failed !\n");
        exit(31);
    }
    usleep(0.6 * SLEEP_IN_S);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

int main(int argc, char* argv[]) {
    for (int counter = 0; counter < 2; ++counter)
        display_proc_self_status();

    printf("========================================================================\n");
    printf("--- About to stack-allocate a 1 Mb buffer\n");
    char BUF[MEG] = {0};
    printf("========================================================================\n");
    usleep(0.6 * SLEEP_IN_S);

    {
        for (int counter = 0; counter < 2; ++counter)
            display_proc_self_status();

        printf("========================================================================\n");
        printf("--- About to write on the stack-allocated 1Mb buffer\n");
        memset(BUF, 'M', MEG);
        printf("========================================================================\n");
        usleep(0.6 * SLEEP_IN_S);

        for (int counter = 0; counter < 2; ++counter)
            display_proc_self_status();

        printf("========================================================================\n");
        printf("--- About to deallocate stack-allocated 1Mb buffer (by leaving scope)\n");
        printf("========================================================================\n");
        usleep(0.6 * SLEEP_IN_S);
    }

    for (int counter = 0; counter < 2; ++counter)
        display_proc_self_status();

    printf("========================================================================\n");
    printf("--- About to malloc 1Mb\n");
    void* malloced = malloc(MEG);
    printf("========================================================================\n");
    usleep(0.6 * SLEEP_IN_S);

    for (int counter = 0; counter < 2; ++counter)
        display_proc_self_status();

    printf("========================================================================\n");
    printf("--- About to write malloced area\n");
    memset(malloced, 'Q', MEG);
    printf("========================================================================\n");
    usleep(0.6 * SLEEP_IN_S);

    for (int counter = 0; counter < 2; ++counter)
        display_proc_self_status();

    printf("========================================================================\n");
    printf("--- About to free malloced area\n");
    free(malloced);
    printf("========================================================================\n");
    usleep(0.6 * SLEEP_IN_S);

    for (int counter = 0; counter < 2; ++counter)
        display_proc_self_status();

    return 0;
}

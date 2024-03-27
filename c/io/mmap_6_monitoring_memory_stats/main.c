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

#define MEG 1048576  // 1024 kio = 1 Mio

void display_proc_self_statm() {
    char* the_file = "/proc/self/statm";
    int fd = open(the_file, O_RDONLY, 0755);
    if (fd < 0) {
        printf("open failed !\n");
        exit(10);
    }

    char buf[1024] = {0};
    int actually_read = read(fd, buf, 1024);
    if (actually_read <= 0) {
        printf("read failed !\n");
        exit(20);
    }

    printf("My own PID is %i\n", (int)getpid());
    /* printf("Raw '%s' is %s\n", the_file, buf); */
    printf("ATTENTION : le résultat de base est donné en NOMBRE DE PAGES, chaque page fait : %i octets \n",
           getpagesize());
    int VmSize, VmRSS, shared_ie_backed_by_a_file, text_ie_code, datastack, ignored;
    sscanf(buf, "%d %d %d %d %d %d", &VmSize, &VmRSS, &shared_ie_backed_by_a_file, &text_ie_code, &ignored, &datastack);
    printf("VmSize                     = %-6i |  x4 = %-6i kB  (assuming 1 page == 4 kB)\n", VmSize, 4 * VmSize);
    printf("VmRSS                      = %-6i |  x4 = %-6i kB  (assuming 1 page == 4 kB)\n", VmRSS, 4 * VmRSS);
    printf("shared_ie_backed_by_a_file = %-6i |  x4 = %-6i kB  (assuming 1 page == 4 kB)\n",
           shared_ie_backed_by_a_file,
           4 * shared_ie_backed_by_a_file);
    printf("text_ie_code               = %-6i |  x4 = %-6i kB  (assuming 1 page == 4 kB)\n",
           text_ie_code,
           4 * text_ie_code);
    printf("data_plus_stack            = %-6i |  x4 = %-6i kB  (assuming 1 page == 4 kB)\n", datastack, 4 * datastack);

    int rc = close(fd);
    if (rc != 0) {
        printf("close failed !\n");
        exit(30);
    }
}

void display_proc_self_status() {
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

    printf("My own PID is %i\n", (int)getpid());
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
}

void* mmap_file(char* input_file) {
    int fd = open(input_file, O_RDONLY, 0755);
    if (fd < 0) {
        printf("open failed !");
        exit(10);
    }

    int mmapped_size = MEG;

    printf("\n========================================================================\n");
    printf("--- About to mmap a file of size : %i kB\n", (mmapped_size / 1024));
    sleep(0.5);
    void* addr = mmap(0, mmapped_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        printf("mmap failed : %s", strerror(errno));
        exit(1);
    }
    printf("--- just mmapped file !\n");
    printf("\n========================================================================\n");
    printf("\n");
    sleep(0.5);
    return addr;
}

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    char* input_file = argv[1];

    for (int counter = 0; counter < 3; ++counter) {
        printf("\n");
        display_proc_self_status();
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        display_proc_self_statm();
        sleep(1);
    }

    void* mmapped = mmap_file(input_file);

    for (int counter = 0; counter < 2; ++counter) {
        printf("\n");
        display_proc_self_status();
        sleep(1);
    }

    printf("\n========================================================================\n");
    printf("--- About to allocate a 1 Mb buffer\n");
    char BUF[MEG] = {0};
    printf("--- Just stack-allocated a 1 Mb buffer\n");
    printf("========================================================================\n\n");
    sleep(1);

    for (int counter = 0; counter < 2; ++counter) {
        printf("\n");
        display_proc_self_status();
        sleep(1);
    }

    printf("\n========================================================================\n");
    printf("--- About to copy 1Mb from the mmaped file to the stack-allocated buffer\n");
    memcpy(BUF, mmapped, MEG);
    printf("--- Just stack copied 1 Mb to the stack-allocated buffer\n");
    printf("========================================================================\n\n");
    sleep(1);

    for (int counter = 0; counter < 2; ++counter) {
        printf("\n");
        display_proc_self_status();
        sleep(1);
    }

    printf("\n========================================================================\n");
    printf("--- About to modify the 1Mb buffer\n");
    memset(BUF, 'M', MEG);
    printf("--- Just modified the 1Mb buffer\n");
    printf("========================================================================\n\n");
    sleep(1);

    for (int counter = 0; counter < 2; ++counter) {
        printf("\n");
        display_proc_self_status();
        sleep(1);
    }

    printf("\n========================================================================\n");
    printf("--- About to modify the 1Mb mmapped file\n");
    memset(mmapped, 'M', MEG);
    printf("--- Just modified the 1Mb mmapped file\n");
    printf("========================================================================\n\n");
    sleep(1);

    for (int counter = 0; counter < 2; ++counter) {
        printf("\n");
        display_proc_self_status();
        sleep(1);
    }

    return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    sleep(2);  // seconds
    pid_t ppid = getppid();
    sleep(2);  // seconds
    return (int)ppid;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    sleep(2);  // seconds
    pid_t ppid = getppid();
    sleep(2);  // seconds
    return (int)ppid;
}

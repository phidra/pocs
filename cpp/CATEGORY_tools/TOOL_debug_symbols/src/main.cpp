#include <cstdlib>

int __attribute__((noinline)) parse(char* arg) {
    return atoi(arg);
}

int main(int argc, char* argv[]) {
    return parse(argv[1]) + parse(argv[2]);
}

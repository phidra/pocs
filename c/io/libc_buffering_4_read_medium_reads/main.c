#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define CONTENT_LEN 1024
char CONTENT[CONTENT_LEN];
int LOOPSIZE = 16;

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    memset(CONTENT, 0, CONTENT_LEN);

    char* TEMPFILE = argv[1];
    printf("\n[C PROGRAM] I will now fread %s\n", TEMPFILE);
    FILE* f = fopen(TEMPFILE, "r");

    sleep(2);  // seconds

    for (int counter = 0; counter < LOOPSIZE; ++counter) {
        fread(CONTENT, CONTENT_LEN, 1, f);
        usleep(500 * 1000);  // half-second
    }

    sleep(2);  // seconds
    printf("\n[C PROGRAM] that's done...\n");
    printf("\n[C PROGRAM] I'm about close file\n\n");
    if (fclose(f)) {
        printf("\n\n[C PROGRAM] Error while closing file !\n\n");
    }

    return 0;
}

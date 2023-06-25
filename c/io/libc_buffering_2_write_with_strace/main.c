#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define LENGTH 1024
char CONTENT[LENGTH];
int LOOPSIZE = 18;

int main(int argc, char* argv[]) {
    assert((4 * LENGTH) < BUFSIZ);  // this POC aims to demonstrate buffering effect...
    assert((LOOPSIZE * LENGTH) > (2 * BUFSIZ));
    assert(argc >= 2);
    memset(CONTENT, 'x', LENGTH);
    CONTENT[LENGTH] = '\0';

    char* TEMPFILE = argv[1];
    printf("\n[C PROGRAM] BUFSIZ = %d", (int)BUFSIZ);
    printf(
        "\n[C PROGRAM] I will now fwrite every half-second, for %d times, a small content (%d bytes) to tempfile : "
        "%s\n",
        LOOPSIZE, (int)LENGTH, TEMPFILE);
    FILE* f = fopen(TEMPFILE, "w");
    sleep(2);  // seconds

    for (int counter = 0; counter < LOOPSIZE; ++counter) {
        fwrite(CONTENT, LENGTH, 1, f);
        usleep(500 * 1000);  // half-second
    }

    sleep(3);  // seconds
    printf("\n[C PROGRAM] that's done...\n");

    // closing stream -> writing file
    printf("\n[C PROGRAM] I'm about close file thus the file will REALLY be written !\n\n");
    if (fclose(f)) {
        printf("\n\n[C PROGRAM] Error while closing file !\n\n");
    }
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    char* TEMPFILE = argv[1];

    // creating a string filled with 'x' of length BUFSIZ+42 :
    size_t LENGTH = BUFSIZ + 42;
    char CONTENT[LENGTH + 1];
    memset(CONTENT, 'x', LENGTH);
    CONTENT[LENGTH] = 0;

    assert(LENGTH > BUFSIZ);  // this POC aims to demonstrate buffering effect...
    sleep(2);                 // seconds

    printf("\n[C PROGRAM] I will now write big content (%d bytes) to tempfile : %s\n", (int)LENGTH, TEMPFILE);
    printf("[C PROGRAM] BUFSIZ is %d bytes\n", BUFSIZ);

    FILE* f = fopen(TEMPFILE, "w");
    fwrite(CONTENT, LENGTH, 1, f);
    printf(
        "[C PROGRAM] write is done, we expect to see a non-empty file (but not full yet) containing BUFSIZ "
        "bytes...\n\n");
    sleep(3);  // seconds

    // closing stream -> writing file
    printf("\n[C PROGRAM] I'm about close file thus the remainder of the file will be written !\n\n");
    if (fclose(f)) {
        printf("\n\n[C PROGRAM] Error while closing file !\n\n");
    }
    return 0;
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    char* TEMPFILE = argv[1];
    char* CONTENT = "coucou le monde !";
    size_t LENGTH = strlen(CONTENT);
    assert(LENGTH < BUFSIZ);  // this POC aims to demonstrate buffering effect...
    sleep(2);                 // seconds

    printf("\n[C PROGRAM] I will now write small content (%d bytes) to tempfile : %s\n", (int)LENGTH, TEMPFILE);

    FILE* f = fopen(TEMPFILE, "w");
    fwrite(CONTENT, LENGTH, 1, f);
    printf("[C PROGRAM] write is 'done', but due to buffering, nothing is expected to happen...\n\n");
    sleep(3);  // seconds

    // closing stream -> writing file
    printf("\n[C PROGRAM] I'm about close file thus the file will REALLY be written !\n\n");
    if (fclose(f)) {
        printf("\n\n[C PROGRAM] Error while closing file !\n\n");
    }
    return 0;
}

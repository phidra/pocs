#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CONTENT_LEN 16
#define RESULT_LEN 256
char CONTENT[CONTENT_LEN];
char RESULT[RESULT_LEN];
int LOOPSIZE = 16;

int main(int argc, char* argv[]) {
    assert(argc >= 2);
    memset(CONTENT, 0, CONTENT_LEN);
    memset(RESULT, 0, RESULT_LEN);

    char* TEMPFILE = argv[1];
    printf("\n[C PROGRAM] I will now fread %s\n", TEMPFILE);
    FILE* f = fopen(TEMPFILE, "r");

    sleep(2);  // seconds

    for (int counter = 0; counter < LOOPSIZE; ++counter) {
        fread(CONTENT, CONTENT_LEN, 1, f);
        usleep(500 * 1000);                                            // half-second
        memcpy(&RESULT[counter * CONTENT_LEN], CONTENT, CONTENT_LEN);  // storing result to display it
    }

    RESULT[CONTENT_LEN * LOOPSIZE] = '\0';

    sleep(2);  // seconds
    printf("\n[C PROGRAM] that's done...\n");
    printf("\n[C PROGRAM] I'm about close file\n\n");
    if (fclose(f)) {
        printf("\n\n[C PROGRAM] Error while closing file !\n\n");
    }

    printf("Result of all the freads : \n");
    printf("%s", RESULT);

    return 0;
}

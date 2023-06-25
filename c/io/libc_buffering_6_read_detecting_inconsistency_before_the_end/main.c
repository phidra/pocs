#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define CHUNK_LEN 16
#define RESULT_LEN 1024
char CHUNK[CHUNK_LEN];
char RESULT[RESULT_LEN];

int main(int argc, char* argv[]) {
    assert(argc >= 3);
    memset(CHUNK, 0, CHUNK_LEN);
    memset(RESULT, 0, RESULT_LEN);

    char* TEMPFILE = argv[1];
    char* RESULTFILE = argv[2];
    printf("\n[C PROGRAM] I will now fread chunk by chunk this file : %s\n", TEMPFILE);
    printf("[C PROGRAM] I will write the results to this file : %s\n", RESULTFILE);
    FILE* f = fopen(TEMPFILE, "r");

    sleep(2);  // seconds

    // note : for this POC, assumes no read error...
    size_t actually_read = fread(CHUNK, 1, CHUNK_LEN, f);
    char* result_ptr = RESULT;
    while (actually_read == CHUNK_LEN) {
        usleep(500 * 1000);  // half-second
        memcpy(result_ptr, CHUNK, CHUNK_LEN);
        result_ptr += actually_read;
        assert(result_ptr < (RESULT + RESULT_LEN));
        actually_read = fread(CHUNK, 1, CHUNK_LEN, f);
    }

    memcpy(result_ptr, CHUNK, actually_read);
    result_ptr += actually_read;

    sleep(2);  // seconds
    printf("\n[C PROGRAM] that's done...\n");
    printf("[C PROGRAM] I'm about to close file\n\n");
    if (fclose(f)) {
        printf("\n\n[C PROGRAM] Error while closing file !\n\n");
    }

    printf("[C PROGRAM] Result of all the freads : \n");
    printf("%s", RESULT);

    printf("[C PROGRAM] Writing results to : %s\n", RESULTFILE);
    FILE* f2 = fopen(RESULTFILE, "w");
    fprintf(f2, "%s", RESULT);
    fclose(f2);

    return 0;
}

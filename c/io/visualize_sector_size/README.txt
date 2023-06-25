This POC tries to show the effect of hard drive physical sector size.

It repeatedly swaps CHUNK_SIZE bytes in a file at positions 0 and 8192.

EXPECTED :
    quick run when CHUNK_SIZE is smaller or equal to sector size
    slower run when CHUNK_SIZE is (a bit) bigger than sector size

GOT : that's a failure : run durations are more or less identical...

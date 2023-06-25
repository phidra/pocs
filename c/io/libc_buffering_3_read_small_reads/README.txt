This POC shows that there is no physical read from disk (or page-cache) at each call to fread.

The code repetively calls very small (16 bytes) freads, with a sleep in between each.
Execution is done in strace to see what are the actual syscalls made.

EXPECTED IN STRACE OUTPUT :
    only two read syscalls made, one at the beginning, the other at the end

GOT : only 1 read syscall made ?!

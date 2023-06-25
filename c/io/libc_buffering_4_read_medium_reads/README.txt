This POC shows that there is no physical read from disk (or page-cache) at each call to fread.

The code repetively calls medium (1024 bytes) freads, with a sleep in between each.
Execution is done in strace to see what are the actual syscalls made.

In the code there is an equal number of calls to fread and usleep.
But in strace, we see that there are actually MORE usleep syscalls thant read syscalls.

Interpretation : not every fread call generates a read syscall -> a.k.a read is buffered :-)


EXPECTED IN STRACE OUTPUT :
    less read syscalls than nanosleep syscalls

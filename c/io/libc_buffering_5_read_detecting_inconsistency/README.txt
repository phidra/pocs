This POC shows that bufferized fread are robust to additions at the end of the file.

We could have thought that as read are bufferized, they won't "see" an addition, but that's not the case.
The probable cause is a final read syscall that is able to tell that the file has been modified since first read syscall.
It seems that while the "final" read syscall is non-empty, we continue to (bufferized) read.

FIRST CASE :
    a file is read small (16b) chunk by small chunk
    strace shows that only one syscall read is made at the beginning, and another at the end
    the read chunks are aggregated and written to a result file
    EXPECTED RESULT = result file is identifical to the source

SECOND CASE :
    same, but in between the chunk by chunk read, we add a mall text at the end of the file
    strace shows that there is an additional read sycall, that "detects" the small added text
    EXPECTED RESULT = result file is STILL (!) identifical to the source

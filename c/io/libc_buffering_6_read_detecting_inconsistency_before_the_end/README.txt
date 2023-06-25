This POC shows that bufferized fread ARE NOT ROBUST to additions before the end of the file.

If we modify the file AFTER that the read buffer has been filled with an actual read syscall,
the subsequent freads WON'T SEE the modification !

FIRST CASE :
    a file is read small (16b) chunk by small chunk
    strace shows that only one syscall read is made at the beginning, and another at the end
    the read chunks are aggregated and written to a result file
    EXPECTED RESULT = result file is identifical to the (non-modified) source

SECOND CASE :
    same, but in between the chunk by chunk read, we modify the source file, few bytes BEFORE the end
    EXPECTED RESULT = result file is different than the source (which have been modified)
    actually, the result file is identical to the source BEFORE the modification

This POC shows that there is no physical write to disk (or page-cache) at each call to fwrite.

Several small writes (with fwrite) are done consecutively, with a sleep between each.
The execution is made in strace to keep an eye on syscalls.

EXPECTED IN STRACE OUTPUT:
    more nanosleep than write syscalls
    write syscalls called with a bigger buffer size (e.g. 4096) than fwrite call buffer size

This POC shows the basics of mmap file read.

It creates a ~16K file, and read 16 times a 64 buffer from it : 
    bytes 0 -> 63
    bytes 1024 -> (1024+63)
    bytes 2048 -> (2048+63)
    ...

The initial intent was to see if "behind the scenes", mmap was calling read.
Spoiler alert : this is not the case :-)

EXPECTED IN STRACE OUTPUT :
    the only "read" that appears is the one to read the elf file
    there is only one mmap call after the "sleep"

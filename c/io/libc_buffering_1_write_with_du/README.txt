This POC shows that there is no physical write to disk (or page-cache)
until libc's buffer is either full or flushed.

CASE 1 = it writes content that is smaller than libc's buffer.
CASE 2 = it writes content that is bigger than libc's buffer.

EXPECTED :
    small write:  du -sb shows an empty file, until buffer is really flushed at file close.
    big write  :  du -sb shows a non-empty (but incomplete) file of size BUFSIZ, until buffer is flushed.

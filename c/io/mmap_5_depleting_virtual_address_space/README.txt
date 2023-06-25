Cette POC montre qu'on peut saturer le virtual address space alors même qu'on n'alloue pas de RAM.

Ce que fait la POC :
    - créée un fichier de ~100 Mio
    - essaye de mmapper le fichier dans le VAS du process, en boucle, 50 fois

Ce qui se passe :
    après avoir mmappé le fichier 30 fois (3000 Mio de VAS consommés), le mmap suivant plante
    en fait, même si mmap n'alloue pas de RAM (du moins pas tant qu'on n'accède pas à la zone allouée)...
    ... le fichier est mmappé sur une range d'addresses virtuelles.
    Or, sur ma machine 32 bits, celles-ci sont limitées à 2^32 = 4 Gio, dont une partie est réservée pour le kernel
    (comme la POC plante à 3 Gio, j'en déduis que 1 Gio est réservé pour le kernel)
    du coup, quand il ne reste plus assez d'addresses virtuelles de libres pour mmapper un fichier de 100Mio, ça plante


Cette POC démontre le monitoring de la mémoire d'un process et l'impact d'un mmap d'un fichier de 1024 kB.

Ce que fait la POC :
    elle parse /proc/self/statm et affiche les statistiques pertinentes
    elle parse /proc/self/status et affiche les statistiques pertinentes
    elle affiche 2 fois les statistiques mémoires
    elle mmappe un fichier de 1 Mio = 1024 kio, puis raffiche les nouvelles stats mémoire
    elle stack-allocate un buffer de 1024 kio, puis raffiche les nouvelles stats mémoire
    elle copie depuis le fichier mmappé vers le buffer, puis raffiche les nouvelles stats mémoire
    elle modifie le buffer, puis raffiche les nouvelles stats mémoire
    elle modifie le fichier mmappé (pour trigger le COW), puis raffiche les nouvelles stats mémoire

Exemple typique de sortie du monitoring mémoire :

    Virtual memory size   | VmSize:     3236 kB
    Resident set size     | VmRSS:       512 kB
    Resident anonymous mem| RssAnon:              48 kB
    Resident file mappings| RssFile:             464 kB
    Resident shared mems  | RssShmem:              0 kB
    Size of data segment  | VmData:      164 kB
    Size of stack segment | VmStk:       136 kB
    Size of text segment  | VmExe:         8 kB
    Shared lib code size  | VmLib:      1876 kB
    Swapped-out virt mem  | VmSwap:        0 kB
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ATTENTION : le résultat de base est donné en NOMBRE DE PAGES, chaque page fait : 4096 octets
    VmSize                     = 809    |  x4 = 3236   kB  (assumes 1 page == 4 kB)
    VmRSS                      = 128    |  x4 = 512    kB  (assumes 1 page == 4 kB)
    shared_ie_backed_by_a_file = 116    |  x4 = 464    kB  (assumes 1 page == 4 kB)
    text_ie_code               = 2      |  x4 = 8      kB  (assumes 1 page == 4 kB)
    data_plus_stack            = 75     |  x4 = 300    kB  (assumes 1 page == 4 kB)

Ce que j'apprends :
    - les deux fichiers de mémoire sont cohérents (heureusement !) :
          + /proc/self/statm
          + /proc/self/status
    - la taille du Resident set size (= la quantité de RAM physique utilisée) se répartit entre :
          + addresses backées par des fichiers
          + addresses anonymes (non-backées par des fichiers, e.g. issues de malloc ou mmap anonyme)
          + addresses correspondant à des shared-mems
    - certaines adresses virtuelles ne consomment pas de RAM physique : VmSize > VmRSS
    - lorsqu'on mmappe un fichier, AUCUNE statique ne bouge, à l'exception de VmSize :
          + ça montre bien que l'action de mmapper un fichier NE CONSOMME pas de RAM physique en soi (VmRSS n'augmente pas)
          + en revanche, l'action de mmapper un fichier consomme des ADDRESSES VIRTUELLES dans le VAS (VmSize augmente)
    - lorsqu'on stack-allocate un buffer de 1024 kio :
          + VmSize augmente de 1024 kio
          + VmRSS augmente de plus de 1024 kio, de façon surprenante
          + RssAnn augmente d'un peu moins de 1024 kio ?!
          + RssFile augmente assez fortemetn aussi (~ 900 kio) ?!!!
      - note : VmStk n'augmente pas... pour la bonne raison qu'il a été fixé statiquement par le compilo en fonction des besoins de mon programme
          + (en effet, si j'instancie DEUX buffers de 1024 kio, VmStk double dès le début du programme, alors que les buffers n'ont pas encore été instanciés)
    - lorsqu'on copie depuis le fichier mmappé vers le buffer, aucune statistique n'augmente
          + logique, le buffer était DÉJÀ alloué dans le VAS, et "consommait" DÉJÀ une memory-frame de la RAM physique
    - lorsqu'on modifie le buffer, aucune statistique n'augmente
          + logique, le buffer était DÉJÀ alloué dans le VAS, et "consommait" DÉJÀ une memory-frame DIFFÉRENTE de la memory-frame backant la zone du VAS mmappée
          + dit autrement : il ne peut pas y avoir de CopyOnWrite, car la zone mémoire (du fichier mmappé) était DÉJÀ copiée (dans le buffer en userspace)
    - lorsqu'on modifie la zone mémoire mmappée, les choses sont intéressantes :
          + le VmSize n'augmente pas : la zone de la mémoire virtuelle était DÉJÀ allouée (on ne consomme pas d'adresses virtuelles supplémentaires)
          + le VmRSS augmente d'environ (un peu plus ?!) de 1024 kio \o/
          + en effet, en écrivant sur la zone mémoire mmappée, on a triggé le COW entre :
            1. la memory-frame du page-cache qui a lu le fichier mmappé, qui n'a pas été modifiée
            2. la memory-frame (propre au process car MAP_PRIVATE) vers laquelle pointe la zone mémoire mmappée, qui a été modifiée

Cette POC vérifie la "consommation de RAM" liée à mmap.

Ce que fait la POC :
    en tâche de fond, elle affiche très régulièrement la RAM libre
        pour le moment, je monitore surtout la RAM libre du système en greppant /proc/meminfo :
        grep MemFree /proc/meminfo
        Note importante : le page-cache N'EST PAS considéré comme de la RAM libre, donc si on remplit le page-cache, la mémoire libre diminue.
    elle mmappe un gros (> 1Gio) dans le process
    elle copie bloc de 1 Mio par bloc de 1 Mio le contenu du gros fichier dans un petit buffer de 1 Mio

Deux versions de la POC sont intéressante :
    A. lorsque le fichier mmappé est dans le page-cache
    B. lorsque le fichier mmappé n'est PAS dans le page-cache (e.g. on a vidé le page-cache avant de lancer la POC)

Ce que je constate :
    Version A = quand le fichier mmappé est dans le page-cache :
        - pas de modification de la RAM libre lorsqu'on mmappe le fichier
        - pas de modification de la RAM libre lorsqu'on accède à la zone mémoire mmappée bloc par bloc
        - la lecture du fichier mmappé est très RAPIDE (quelques dixièmes de secondes)
    Version B = quand le fichier mmappé N'EST PAS dans le page-cache :
        - pas de modification de la RAM libre lorsqu'on mmappe le fichier (ça ça ne change pas)
        - la RAM libre diminue fortement lorsqu'on accède à la zone mémoire mmappée bloc par bloc
        - la lecture du fichier mmappé est très LENTE (~25 secondes sur ce PC)

Ce que j'en déduis (à confirmer ?) :
    le mapping à proprement parler n'est que la mise en place d'une correspondance entre le Virtual Address Space et le disque, elle n'utilise pas de RAM
    lorsqu'on accède à une zone mémoire mmappée, tout se passe comme si on lisait le disque
    notamment, si la zone du disque en question n'est pas dans le page-cache, il y a un accès disque, qui remplit le page-cache
    inversement, si la zone mmappée est déjà dans le page-cache, il est utilisé (c'est très rapide), et il n'y a pas d'accès disque

Cette POC fera sans doute des petits pour mieux analyser le comportement ou pour confirmer mes déductions.

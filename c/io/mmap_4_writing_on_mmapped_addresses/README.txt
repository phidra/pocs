Cette POC est la suite de la précédente, et essaye d'observer le lien entre mmap et les pages mémoires.

Ce que fait la POC :
    - elle mmappe un relativement gros (~ 100 Mio) dans le process, en READ+WRITE et MAP_SHARED
    - elle fait une boucle infinie sur les tâches suivantes (à arrêter avec CTRL+C, donc)
    - elle copie bloc de 1 Mio par bloc de 1 Mio le contenu du fichier dans un petit buffer de 1 Mio
    - on mesure le temps pris par chaque groupe de copies -> il est à peu près constant
    - après 5 tours de boucles, on écrit sur la zone mémoire, puis on continue les boucles

Ce que je voulais tester :
    - est-ce que le fait d'écrire sur les pages trigge le copy-on-write, et est plus lent ?

Ce que je constate :
    - le temps des boucles de lecture est à peu près constant, malgré l'écriture qui a eu lieu sur la pages mémoire (et sur le disque aussi)

Ce que j'en déduis :
    - probablement que le COW a lieu pendant l'écriture : au moment où je reprends les lectures en boucles, pas de raison que ce soit plus lent
    - à noter que j'ai essayé aussi en MAP_PRIVATE, et que ça ne change pas le comportement

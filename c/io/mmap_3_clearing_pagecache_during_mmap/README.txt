Cette POC est la suite de la précédente, et essaye d'observer le lien entre mmap et les pages mémoires.

Ce que fait la POC :
    - elle mmappe un relativement gros (~ 100 Mio) dans le process, en RDONLY et MAP_SHARED
    - elle fait une boucle infinie sur les tâches suivantes
    - elle copie bloc de 1 Mio par bloc de 1 Mio le contenu du fichier dans un petit buffer de 1 Mio
    - on mesure le temps pris par chaque groupe de copies -> il est à peu près constant
    1. de façon externe, l'utilisateur vide le page-cache, on regarde si ça impacte le temps
    2. de façon externe, modifier le fichier mmappé, et vérifier si ça impacte le temps

Ce que je voulais tester :
    vérifier que lorsque les memory frames sont modifiées, le traitement est plus lent

Ce que je constate :
    pas d'augmentation significative du temps de lecture de la zone mmappée dans aucun des deux cas

Ce que j'en déduis :
    pas grand chose, faudrait que je comprenne plus finement ce qui se passe
    je suis très surpris que lorsqu'on modifie le fichier de façon externe, la lecture ne soit pas modifiée
    en effet, j'aurais cru que la memory-frame serait alors marquée "dirty", et rechargée en RAM avec les nouvelles valeurs
    ça n'a pas l'air d'être le cas

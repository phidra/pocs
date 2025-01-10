#!/bin/bash
#
set -o errexit
set -o nounset
set -o pipefail

this_script_parent="$(realpath "$(dirname "$0")" )"
echo "parent = $this_script_parent" > /dev/null

rm -rf NOGIT_build
 CXX=clang++-14 meson setup NOGIT_build/ .
 CXX=clang++-14 meson compile  -C NOGIT_build/


# une fois n'est pas coutume, la description de la POC n'est pas dans le main,
# car il y en a plusieurs :
cat << EOF

CE QUE MONTRE CETTE POC = l'utilisation de PImpl

**POSITION DU PROBLÈME :**

- un main qui utilise une app MyApp pour récupérer l'âge d'un utilisateur
- pour cela, l'app utilise une librairie "repo", qui continet tout plein d'infos sur les utilisateurs
- l'app a besoin de maintenir une connexion vers le repo, dans une variable membre privée
- problème : en C++, même les trucs privés d'une classe doivent apparaître dans le header public
- ici, ça nous embête : on se retrouve forcé à dépendre publiquement d'un truc qu'on aurait aimé garder privé


**LA POC :**

- on va compiler deux binaires qui font la même chose, de deux façons différentes
- dans les deux cas, le code-source du main, et le code-source du repo sont absolument IDENTIQUES
- ce qui change, c'est le code-source de l'app qui utilise le repo :
- WITHOUT_PIMPL :
    - dans le header 'myapp.h', la classe MyApp a un membre privé DataRepoConnection qui permet d'utiliser le repo
    - du coup, l'app dépend PUBLIQUEMENT de la librairie 'repo'
    - conséquemment, le main a connaissance de la librairie 'repo'
- WITH_PIMPL :
    - dans le header 'myapp.h', la classe MyApp a un membre privé opaque PImpl
    - ça n'est que dans 'myapp.cpp' que PImpl a lui-même un membre privé DataRepoConnection
    - du coup, publiquement, l'app ne dépend plus de rien ; la dépendance de l'app à la librairie 'repo' est PRIVÉE
    - conséquemment, le main n'a pas connaissance de la librairie 'repo'
- on peut observer la dépendance (ou non) du main à la librairie 'repo' en regardant sa commande compilation

**PIMPL — PRINCIPE :**

- la classe ne stocke qu'un pointeur OPAQUE (i.e. vers une structure qui n'est pas définie dans le header)
    - cf. https://en.wikipedia.org/wiki/Opaque_pointer
- avantage pour ma POC = la structure est définie dans le cpp → on a déplacé la dépendance de PUBLIQUE à PRIVÉE
- avantages :
    - séparation de l'interface (ce qui est utilisable publiquement) et de l'implémentation (illustré par la POC)
    - stabilité de l'ABI, très utile quand c'est une librairie qu'on développe :
        - PImpl permet de modifier les détails d'implémentation de la librairie sans toucher au header public
        - on peut modifier l'implémentation de la lib sans que les utilisateurs aient à recompiler leur code appelant
        - mieux : on peut changer l'ABI du PImpl ! Comme les utilisateurs ne dépendent que de l'ABI de la lib, on reste compatible
    - temps de compilation : même principe = si le pimpl change, seul le cpp l'utilisant devra être recompilé
- inconvénients (cf. le lien de cppreference pour des détails sur les inconvénients) :
    - runtime overhead : les appels doivent passer par le pimpl
    - runtime overhead : fragmentation de la mémoire = extra-allocation pour le pimpl
    - runtime overhead : plus difficile à optimiser, les appels sont dispatchés sur plusieurs translation-units : seule la LTO pourra les optimiser)
    - space overhead : par rapport à une version non-pimpl, il faut stocker un pointeur en plus
    - lifetime management : déjà, il faut construire les objets, ce qui prend un peu plus de temps
    - lifetime management : NDM : ne pas se merdoiller dans la gestion du cycle de vie (e.g. ne pas oublier de delete le pimpl quand son conteneur meurt)
- on en revient encore à une question fondamentale :
    - QUESTION = quel intérêt de déplacer la dépendance de myapp à la lib dans le CPP ?
    - En effet, ça ne change rien : myapp dépendra tout de même de la lib ! Je ne peux pas compiler myapp sans la lib !
    - RÉPONSE = l'intérêt est que la dépendance est maintenant PRIVÉE, alors qu'avant elle était PUBLIQUE !
    - certes, myapp dépend toujours de la lib... mais le main qui utilise myapp, lui, ne dépend plus de la lib !
    - (alors que sans pimpl, il en dépendait aussi, i.e. il en avait connaissance, devait l'inclure, et devait être recompilé si la lib changeait)

**PIMPL — NOTES D'IMPLÉMENTATION :**

- souvent implémenté avec un std::unique_ptr (car l'objet interface contrôle le cycle de vie du pimpl)
    - dans ce cas, il FAUT définir le destructeur dans le cpp, y compris si c'est simplement :
        MyInterface::~MyInterface() = default;
    - (plus de détails dans le lien cppreference et le lien de Jonathan BOCCARA)
    - de même, les opérateurs spéciaux sont à gérer le cas échéant (car la classe interface gère un pointeur)
- si on doit passer quelque chose à la construction du pimpl pointeur, il faut passer le pimpl en paramètre du constructeur de la classe (?)
    - (ça n'est pas mon cas ici)
- on peut ajouter une (non-owning) référence ou un pointeur dans l'autre sens : du pimpl vers la classe parente
    - ça peut être nécessaire si l'une des méthodes du pimpl nécessite d'utiliser un membre ou une fonction publique de la classe parente
- il y a une technique encore plus avancée (googler "fast pimpl") pour ne pas avoir d'extra-allocation

**PIMPL — LIENS :**

- https://herbsutter.com/gotw/_100/
    - ^ c'est celui que je recommande pour se renseigner sur le sujet
    - introductif puis détaillé, très bien
- https://en.cppreference.com/w/cpp/language/pimpl
    - très détaillé (notamment sur les inconvénients !)
    - avantages tournés vers le fait de pouvoir faire des modifs tout en restant ABI compatible
    > if a library uses pImpl in its ABI, newer versions of the library may change the implementation while remaining ABI-compatible with older versions.
    - il y a notamment des détails sur unique_ptr et les conséquences sur le destructeur, les special member fonctions, et la constness
- https://www.fluentcpp.com/2017/09/22/make-pimpl-using-unique_ptr/
    - intéressant surtout pour l'explication alternative sur pourquoi il faut définir le destructeur
- https://stackoverflow.com/questions/8972588/is-the-pimpl-idiom-really-used-in-practice
    - l'une des réponses adresse "pourquoi utiliser pimpl"

EOF


# exécution :
NOGIT_build/bin_WITHOUT_PIMPL
NOGIT_build/bin_WITH_PIMPL

# on montre que la compilation du main dépend ou non du repo, selon qu'on est avec ou sans PIMPL :
echo ""
echo ""
echo "COMPILATION DU MAIN QUI UTILISE REPO (spoiler-alert = c'est celui où myapp n'utilise pas pimpl) :"
grep command compile_commands.json | grep main.cpp | grep --color=auto repo

echo ""
echo ""
echo "COMPILATION DU MAIN QUI N'UTILISE PAS REPO (spoiler-alert = c'est celui où myapp utilise pimpl) :"
grep command compile_commands.json | grep main.cpp | grep --color=auto -v repo

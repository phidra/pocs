* [POC de protection d'une API par un cookie apikey](#poc-de-protection-dune-api-par-un-cookie-apikey)
   * [Position du problème](#position-du-problème)
   * [Utilisation du board](#utilisation-du-board)
   * [Sous le capot](#sous-le-capot)
      * [Board](#board)
      * [Login](#login)
      * [Interface d'administration](#interface-dadministration)
   * [Rotation de la clé](#rotation-de-la-clé)
   * [Utilisation](#utilisation)
      * [Démarrage](#démarrage)
      * [Administration](#administration)
      * [Connexion et affichage du board](#connexion-et-affichage-du-board)
   * [Tests](#tests)
   * [Vérification manuelle du login](#vérification-manuelle-du-login)



## Position du problème

Supposons qu'on veuille afficher un board sur un équipement (e.g. une borne d'information à l'entrée d'un bâtiment).

Ce board requête une API en backend pour récupérer les infos à afficher, et il n'y a pas d'utilisateur humain qui se connecte (pas d'identité humaine).

On accepte d'avoir une étape de configuration à l'installation de la borne (qui forcerait un technicien à remplir un formulaire, par exemple), mais une fois cette étape de configuration effectuée, la borne doit tourner sans nécessiter d'intervention.

On veut éviter que l'API ne soit ouverte publiquement, en introduisant un _premier_ niveau de sécurité = ne laisser passer les requêtes que si elles sont pourvues d'une clé API valide.

(note : il y a beaucoup d'autres choses à mettre en place pour mieux sécuriser ce contexte fonctionnel : réseau dédié, enrollment d'une identité machine non-exportable, limitation des droits d'accès, borne en mode kiosque durci ; mais la présente POC ignore tout ceci et se concentre sur ce cas simple = une clé API statique)

L'un des enjeux de la POC est d'étudier ce qui se passe en cas de _rotation de la clé API_.

## Utilisation du board

Fonctionnellement, il n'y a qu'une étape de connexion à la première utilisation :

- ouvrir la page `/board` : à la première utilisation, on est automatiquement redirigé vers la page de connexion
- le mot de passe de connexion est hardcodé à `pouet`
- une fois connecté, on est redirigé sur le board, qui est consultable librement (même si on ferme/rouvre le navigateur)

## Sous le capot

### Board

Page `/board` :

- requête régulièrement l'API en backend pour mettre à jour les infos
- joint (automatiquement) un éventuel cookie `apikey` ; la page affiche le cookie utilisé
- redirige vers la page de login si ces requêtes sont refusées en 401 ou 403

### Login

Page `/login` :

- attend une page de redirection en paramètre, p.ex.  `/login?redirect=%2Fboard`
- fait une première requête vers `/api/session` pour voir si on est déjà connecté (= on a déjà le bon cookie `apikey`)
- si oui, redirige immédiatement sur la page de redirection
- si non, affiche le formulaire de connexion (pour la POC, un simple password)
- en cas de connexion réussie :
    - sette le cookie `apikey` (avec une durée de vie de 50 ans) contenant la clé API : ça permet aux requêtes ultérieures d'être acceptées
    - redirige sur la page de redirection

### Interface d'administration

Page `/admin` :

- page réservée à la POC (elle n'existerait pas si le board était utilisé sur un vrai équipement)
- affiche la (ou les) clés API actuelles, ainsi que les requêtes reçues, et leur status vis-à-vis de la connexion :
    - page publiquement accessible
    - page protégée, accédée avec la clé API principale
    - page protégée, accédée avec la clé API secondaire
    - page protégée, accès refusé car mauvaise clé
    - page protégée, accès refusé car absence de clé
- permet de rotater la clé (cf. ci-dessous)

Note : en production, il faudrait marquer le cookie `apikey` comme étant `HttpOnly`, et `Secure`. Pour les besoins pédagogiques de cette POC, ça n'est pas le cas, afin que le board puisse afficher sa valeur.

## Rotation de la clé

La rotation de la clé API ne nécessite pas de reconnexion côté front, ça repose sur deux choses côté serveur :

- une phase de transition (temporaire) où deux clés sont acceptées : l'ancienne et la nouvelle
- le fait qu'une requête vers une ressource protégée, réussie grâce à l'ancienne clé, reçoive en réponse la nouvelle clé API en cookie

Ainsi, si on vient de rotater la clé et qu'on est en phase de transition, dès qu'une requête protégée a lieu :

- elle sera acceptée (car on est en phase de transition : le serveur accepte encore l'ancienne clé)
- elle aura pour conséquence de mettre à jour la clé API côté client (car la nouvelle clé est alors renvoyée en header `Set-Cookie`)

Le seul point de vigilance est que le front doit passer au moins une requête vers une ressource protégée pendant que la transition est en cours ; si la période de transition prend fin avant, le cookie ne sera pas mis à jour, et le front devra se reconnecter pour l'obtenir.

## Utilisation

### Démarrage

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python -m backend.app
```

### Administration

Ouvrir <http://127.0.0.1:5000/admin>

Le mode admin permet de consulter les requêtes, et de choisir le mode :

- mode normal = une unique clé API acceptée
- mode transition = deux clés sont acceptées (l'ancienne et la nouvelle) , les requêtes pourvues de l'ancienne clé apparaissent en orange

(on peut transitionner entre les deux modes avec des boutons)

Le (re)chargement de la page `/admin` réinitialise la POC :

- côté serveur, supprime la clé API secondaire, et restaure la clé API principale à `initial-value`
- vide le journal des logs de requêtes
- côté client, supprime le cookie `apikey`

### Connexion et affichage du board

Ouvrir <http://127.0.0.1:5000/board>

La redirection vers la page de login est automatique si nécessaire.

## Tests

```sh
.venv/bin/python -m pytest
```

## Vérification manuelle du login

Note : la page `/admin` doit être chargée _avant_ les tests (car son chargement réinitialise tout).

**Login quand on est anonyme :**

- Ouvrir `/board` sans cookie : le navigateur doit rediriger vers `/login?redirect=%2Fboard`.
- Saisir un mauvais mot de passe : un message d'erreur apparaît et aucune redirection n'a lieu.
- Saisir le bon mot de passe (hardcodé à `pouet`) : le serveur pose le cookie `apikey` et le navigateur revient sur `/board`.

**Login quand on est déjà loggé :**

- Ouvrir `/login?redirect=%2Fboard` quand on est déjà loggé : la redirection vers `/board` est immédiate, sans formulaire visible.

**Rotation de la clé :**

- quand on affiche `/board` sur un onglet et `/admin` sur un autre, et qu'on est déjà loggé (le board est fonctionnel)
- depuis `/admin`, définir une nouvelle clé API et passer en transition
- on doit voir passer une unique requête orange (pourvue d'une ancienne clé) dans le journal des requêtes : la nouvelle clé est aussitôt reçue en réponse à cette requête
- dans `/admin`, cliquer sur "terminer la transition"
- le board reste utilisable, et à aucun moment il n'a été nécessaire de se reconnecter

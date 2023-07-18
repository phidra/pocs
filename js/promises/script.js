async function main() {

  const poc_description = `

Cette POC a pour objectif de mieux comprendre les Promises ; sources :
    https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise
    https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Using_promises
    https://julesblom.com/writing/running-promises-in-parallel

- C'est quoi une Promise, ça répond à quel problème :
    les promises wrappent un traitement asynchrone (i.e. dont on n'aura pas forcément le résultat tout de suite)
    javascript a une IO-loop builtin : les tâches asynchrone s'exécutent en background sans bloquer le thread principal
    si je veux fourir une fonction qui fait un traitement asynchrone, je fais comment ? Par exemple :
        + setTimeout = le client veut exécuter du code après un certain temps
        + fetch = le client veut requêter une ressource HTTP et faire quelque chose avec la réponse
- ancienne façon de faire = je demande au client de passer la callback qu'il souhaite appeler sur la réponse asynchrone quand elle arrive :
    exemples :
        + setTimeout(delay, callback)
        + fetch(url, callback)
    ça n'est pas très pratique, et ça conduit au callback-hell = la difficulté à chaîner des appels asynchrones
    par exemple, si je veux enchaîner 3 appels HTTP à fetch (chacun dépendant de la réponse précédente) :
        fetch(url1, (response1) => {
            fetch(response1.url2), (response2) => {
                fetch(response2.url3, (response3) => {
                    console.log("final msg is ", response3.final_msg);
                }
            }
        });
- a contrario, les promises sont un moyen simple (avec then et catch) d'attacher des handlers à un traitement asynchrone, et à son futur résultat :
    fetch(url1).then(
        (response1) => { fetch(response1.url2); }).then (
        (response2) => { fetch(response2.url3); }).then (
        (response3) => { console.log("final msg is ", response3.final_msg); }
    );
    (je mets "simple" entre guillemets, car je trouve ça beaucoup moins intuitif que async/await)
- du coup, toute fonction moderne qui fait un traitement asynchrone renvoie une Promise wrappant son résultat
- il n'est presque jamais nécessaire de construire une Promise soi-même :
    ça ne sert qu'à wrapper une fonction asynchrone un peu ancienne, qui n'a pas d'API utilisant une Promise, et qui attend une callback à la place
    c'est ce que j'ai fait avec mon sleep autour de setTimeout
        In an ideal world, all asynchronous functions would already return promises.
        Unfortunately, some APIs still expect success and/or failure callbacks to be passed in the old way.


Je comprends beaucoup de choses avec cette POC, et notamment :

- les fonctions async sont un sucre syntaxique pour renvoyer des Promises (qui resolve, ou reject en cas d'exception)
    du coup, elles ne throwent jamais
    du coup, si on les appellent sans await, ça lance leur exécution quand même, mais en tâche de fond = sans attendre le résultat, et ça renvoie une Promise
- await n'est pas propre aux fonctions asynchrones : on peut await une Promise
- la syntaxe équivalente aux fonctions asynchrones mais utilisant les Promise, qui est un peu particulière :
    le constructeur de Promise n'attend qu'un argument = la fonction qui fait le boulot (l'équivalent du corps de la fonction asynchrone)
    celle-ci prend deux arguments = (resolve, reject), qui sont des leviers permettant à la fonction qui fait le boulot de modifier la Promise
    quand elle a finit son boulot, elle appelle resolve pour settle la Promise (ou reject en cs d'erreur
    (et si la fonction throw au lieu d'appeler reject, c'est à peu près pareil = https://stackoverflow.com/questions/33445415/javascript-promises-reject-vs-throw )
- on dirait qu'il n'est pas du tout pratique de connaître le status actuel d'une promise :
    https://dev.to/xnimorz/101-series-promises-2-how-to-get-current-promise-status-and-build-your-own-promise-queue-18j8
- l'intérêt des fonctions asynchrones, c'est de possiblement de faire plusieurs traitements "en parallèle" :
    pour cela, on utilise des fonctions de composition de Promises i.e. de composition de traitement asynchrone
    ces fonctions renvoient une Promise "résumant" l'état de plusieurs autres Promise, selon deux axes orthogonaux :
        doit-on failfast dès qu'une Promise échoue ?
        renvoie-t-on un unique résultat (= celui de la première Promise qui réussit), ou un array des résultats des Promises ?
    les 4 cas possibles pour ces 2 axes sont traités par : .all / .allSettled / .race / .any

Je ressors de tout ceci avec une meilleure compréhension des Promises.
  `;

  console.log(poc_description);

  await scenarioA_appeler_une_fonction_asynchrone_sans_await_execute_la_fonction_et_retourne_une_promise();
  console.log("");

  await scenarioB_on_peut_donc_lancer_plein_de_trucs_en_parallele();
  console.log("");

  await scenarioC_async_et_promises_sont_equivalents_donc_on_peut_await_une_promise();
  console.log("");

  await scenarioD_my_custom_promise();
  console.log("");

  await scenarioE_my_custom_sleep();
  console.log("");
}



// ===============


async function v1_http_call_async() {
  console.log("CALLED = v1_http_call_async");
  const response = await fetch("https://www.google.fr/");
  return response.statusText;
}

// la fonction suivante n'est PAS asynchrone (mais elle se manipule pareil, car elle renvoie une Promise) :
function v2_http_call_promise() {
  console.log("CALLED = v2_http_call_promise");
  const promise = new Promise((resolve, reject) => {
    fetch("https://www.google.fr/").then((response) => resolve(response.statusText), () => {console.log("BOUM") ; reject() });
  });
  return promise;
}


async function scenarioA_appeler_une_fonction_asynchrone_sans_await_execute_la_fonction_et_retourne_une_promise() {
  const return_value = v1_http_call_async();  // pas d'await ici !
  console.log("SCENARIO A> ce que retourne une fonction asynchrone sans async est : ", return_value.constructor.name);  // Promise
  await return_value;  // avant de quitter, on cleane les promises qui trainent...
}


async function scenarioB_on_peut_donc_lancer_plein_de_trucs_en_parallele() {
  console.log("SCENARIO B> About to create all promises...");
  const promise1 = v1_http_call_async();  // pas d'await ici !
  const promise2 = v1_http_call_async();  // pas d'await ici !
  const promise3 = v1_http_call_async();  // pas d'await ici !
  const promise4 = v2_http_call_promise();
  const promise5 = v2_http_call_promise();
  const promise6 = v2_http_call_promise();

  console.log("SCENARIO B> About to wait for all promises...");
  const all = await Promise.all([
    promise1,
    promise2,
    promise3,
    promise4,
    promise5,
    promise6,
  ]);
  console.log("SCENARIO B> finished !");

}


async function scenarioC_async_et_promises_sont_equivalents_donc_on_peut_await_une_promise() {
  // du coup, on peut utiliser une fonction qui retourne une promise comme si c'était une fonction asynchrone :
  const got1 = await v1_http_call_async();
  console.log("SCENARIO C> GOT 1 = ", got1);

  const got2 = await v2_http_call_promise();
  console.log("SCENARIO C> GOT 2 = ", got2);
}


function my_custom_promise(what_should_i_do) {  // SUCCEEDS OR FAILS
  // Une promise permet de wrapper un traitement potentiellement coûteux.
  // La syntaxe est un chouille particulière : le constructeur de la promise prend un uniquement argument :
  //    = une fonction qui accepte (resolve, reject), et fait son traitement
  //    au sein de cette fonction, on fait ce qu'on veut
  //    et quand on le souhaite, i.e. quand le traitement est terminé, on appelle resolve (ou reject en cas d'erreur)
  return new Promise ((resolve, reject) => {
    console.log("SCENARIO D> inside custom promise 3");
    if (what_should_i_do === "success") {
      resolve();
    } else {
      reject();
    }
  });
}


async function scenarioD_my_custom_promise() {
  my_custom_promise("success").then((result) => { console.log("SCENARIO D> appel 1 = success"); }).catch((error) => {console.log("SCENARIO D> appel 1 = failure"); });
  my_custom_promise("failure").then((result) => { console.log("SCENARIO D> appel 2 = success"); }).catch((error) => {console.log("SCENARIO D> appel 2 = failure"); });


  // (ici, je triche un peu : pour ne pas parasiter le scénario suivant, je dors un peu) :
  await sleep(1000);
}


function sleep(ms_to_sleep) {
  // Autre exemple du point précédent = le traitement fait dans la promesse peut être de dormir.
  // Du coup, on se retrouve avec une fonction asynchrone qu'on peut await pour sleep :
  return new Promise ((resolve, reject) => {
    setTimeout(() => { resolve(); }, ms_to_sleep);
  });
}


async function scenarioE_my_custom_sleep() {
  console.log("SCENARIO E> avant mon sleep (qui va dormir 2 secondes)");
  await sleep(2000);
  console.log("SCENARIO E> après mon sleep");
}

main();

async function main() {

  const poc_description = `

Cette POC a pour objectif de mieux comprendre les Promises.

Je vois beaucoup de choses avec cette POC, et notamment :

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
- quand on a plusieurs Promise, on peut récupérer une Promise "résumant" l'état des autres, selon deux axes orthogonaux :
    doit-on failfast dès qu'une Promise échoue ?
    renvoie-t-on un unique résultat (= celui de la première Promise qui réussit), ou un array des résultats des Promises ?
    les 4 cas possibles sont traités par : .all / .allSettled / .race / .any
    cf. https://julesblom.com/writing/running-promises-in-parallel

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

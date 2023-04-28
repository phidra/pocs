import * as https from 'node:https';
import * as http from 'node:http';


function https_request_with_callbacks(
  url: string,
  process_response: ((response: string) => void),
  process_error: ((error: Error) => void),
): void {
  https.get(url, (response: http.IncomingMessage) => {
    // // A ESSAYER = décommenter cette ligne pour vérifier quand cette callback qui gère l'IncomingMessage est appelée)
    // console.log("Look Mom, I'm binding callbacks to 'data' and 'end' events, yay !");
    let received = '';

    response.on('data', (chunk: string) => {
      received += chunk;
      console.log("receiving !");
    });

    response.on('end', () => {
      process_response(received);
    });

  }).on("error", process_error);
}


function main() {
  console.log("======= LOG 1");
  function process_response(response: string): void {
    const json_response = JSON.parse(response);
    console.log(json_response["explanation"]);
  }
  function process_error(error: Error): void {
    console.error(error);
  }

  const url = "https://api.nasa.gov/planetary/apod?api_key=DEMO_KEY";
  https_request_with_callbacks(url, process_response, process_error);

  console.log("======= LOG 2 (dans le code-source, ce log est appelé *après* le lancement de la requête HTTP, pour vérifier si l'exécution est (a)synchrone)");


  const NOTES_SUR_LA_POC = `
  CONSTATÉ = utiliser http(s).get classiquement, avec des callbacks, revient à être en mode fire-and-forget :
    - on exécute une fonction synchrone (sans await) qui lance une requête HTTP asynchrone
    - (mon modèle mental = l'envoi de la requête a été ajouté à l'event-loop, ainsi surtout que l'attente de sa réponse)
    - la fonction synchrone rend la main aussitôt la requête HTTP lancée (on voit s'afficher le log suivant sans attendre la réponse HTTP)
    - on a bindé des callbacks sur les events de réception de réponse HTTP ou réception d'erreur
    - du coup lorsque l'event-loop résout la requête HTTP (on a reçu la réponse), elle exécute nos callbacks

  Attention que c'est un peu confusant parce que le moyen de binder les callbacks qu'on veut voir exécutées à la réception de la requête HTTP...
  ... c'est d'utiliser une callback ! Du coup il y a deux types de callbacks :
    - type 1 = mes callbacks appelées lorsque la requête HTTP est résolue (e.g. l'event "end" va trigger ma callback "process_response")
    - type 2 = la callback appelée lors de la création de la requête, qui n'est qu'un détail technique servant à binder mes callbacks

  TL;DR = on lance la requête HTTP en asynchrone, sans arrêter l'exécution du script ; la façon pour moi de récupérer / utiliser sa réponse, c'est de binder une callback sur sa réception.

  Variantes de la POC à tester manuellement en éditant le code :
    1. utiliser un host invalide pour constater qu'on appelle process_error
    2. appeler des trucs CPU-intensive pour retarder artificiellement la fin de la POC (et voir quand sont traités les events HTTP)
    3. logger à l'appel de la callback de type 2 pour voir quand elle est appelée (spoiler alert : pas immédiatement)

  Pour les pérenniser, j'ajoute ici mes notes brutes sur le fonctionnement de http(s) :
      https://nodejs.org/api/https.html#httpsgetoptions-callback
          https.get(url[, options][, callback])
          Like http.get() but for HTTPS.
      https://nodejs.org/api/http.html#httpgetoptions-callback
          The callback is invoked with a single argument that is an instance of http.IncomingMessage.
      https://nodejs.org/api/http.html#class-httpincomingmessage
          Extends: <stream.Readable>
          An IncomingMessage object is created by http.Server or http.ClientRequest and passed as the first argument to the 'request' and 'response' event respectively. It may be used to access response status, headers, and data.
      https://nodejs.org/api/stream.html#class-streamreadable
          The 'data' event is emitted whenever the stream is relinquishing ownership of a chunk of data to a consumer.
          (...)
          The 'end' event is emitted when there is no more data to be consumed from the stream.
      Donc si je me résume :
          http.get accepte en argument une callback qui recevra un http.IncomingMessage
          lorsqu'on appelle http.get, on envoie la requête sur le réseau, on crée un http.IncomingMessage destiné à contenir la réponse à la requête, on appelle la callback dessus, puis on envoie la requête sur le réseau
          IncomingMessage hérite de stream.Readable
          la callback binde différentes réactions aux events data / end / error de l'IncomingMessage représentant la réponse à la requête
          la requête HTTP tourne en tâche de fond, l'OS/le moteur JS réceptionne les données ; lorsque l'implémentation du requester HTTP par le moteur JS réceptionne suffisamment de données, il émet l'event "data"
              l'émission de l'event "data" trigge ce qu'on lui a bindé (à savoir l'enregistrement des données dans une string)
          lorsque l'implémenation du requester HTTP constate qu'il a fini de réceptionner l'intégralité de la réponse, il émet l'event "end"
              l'émission de l'event "end" trigge ce qu'on lui a bindé (à savoir la résolution de la promesse)
      L'error est bindée à ce qui est retourné par https.get à savoir  :
          https://nodejs.org/api/https.html#httpsgetoptions-callback
              Like http.get() but for HTTPS.
          https://nodejs.org/api/http.html#httpgetoptions-callback
              Returns: <http.ClientRequest>
          https://nodejs.org/api/http.html#class-httpclientrequest
              Extends: <http.OutgoingMessage>
          https://nodejs.org/api/http.html#class-httpoutgoingmessage
              Extends: <Stream>
      En résumé, on binde data/end au stream de l'incomming message, mais on binde error au stream de la requête
  `;

  console.log("");
  console.log(NOTES_SUR_LA_POC);


  // // A ESSAYER = décommenter pour retarder la fin de la POC avec des calculs (et voir quand sont traités les events HTTP) :
  // function fib(n: number): number {
  //   if (n == 1 || n == 2) {
  //     return 1;
  //   }
  //   return fib(n - 1) + fib(n - 2);
  // }
  // console.log("fib(10) = ", fib(10));
  // // console.log("fib(20) = ", fib(20));
  // // console.log("fib(30) = ", fib(30));
  // // console.log("fib(39) = ", fib(39));
  // // console.log("fib(40) = ", fib(40));
  // // console.log("fib(41) = ", fib(41));
}

main();


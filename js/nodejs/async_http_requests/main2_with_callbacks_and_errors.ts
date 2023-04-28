import * as https from 'node:https';
import * as http from 'node:http';


function https_request_with_callbacks(
  url: string,
  process_response: ((response: string) => void),
  process_error: ((error: Error) => void),
): void {
  https.get(url, (response: http.IncomingMessage) => {
    let received = '';

    response.on('data', (chunk: string) => {
      received += chunk;
      console.log("receiving !");
    });

    response.on('end', () => {
      process_response(received);
      // // A ESSAYER = décommenter ces lignes pour tester différentes façons de remonter des errerus :
      // process_error(new Error("Do this if you must handle an error"));
      // throw new Error("Don't do this = don't throw exception in asynchronous callbacks !");
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

  console.log("======= LOG 2 (ce log est appelé *après* le lancement de la requête HTTP, pour vérifier si l'exécution est (a)synchrone)");

  // on trigge un autre event plus tardif pour vérifier s'il est exécuté même en cas d'erreur :
  setTimeout(() => console.log("... I was DELAYED ! Do I still display ?"), 4000);

  const NOTES_SUR_LA_POC = `
  CONSTATÉ :
    - la façon propre de gérer des erreurs au sein des callbacks de réception HTTP n'est PAS de throw une exception
    - (en effet, dans ce cas, c'est l'ensemble du script qui est interrompu)
    - la façon propre est plutôt d'appeler sa propre callback d'erreur (ici, process_error)

  Pour le vérifier, on simule une erreur HTTP et on regarde si le log "I was DELAYED" est affiché ou pas.
  `;

  console.log("");
  console.log(NOTES_SUR_LA_POC);

}

main();


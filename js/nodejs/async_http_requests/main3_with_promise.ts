import * as https from 'node:https';
import * as http from 'node:http';


async function https_request_with_async_await(url: string): Promise<string> {
  return new Promise((resolve, reject) => {
    https.get(url, (response: http.IncomingMessage) => {
      let data = '';

      response.on('data', (chunk: string) => {
        data += chunk;
      });

      response.on('end', () => {
        resolve(data);
      });

    }).on("error", (error: Error) => {
      reject(new Error(error.message));
    });
  });
}


async function main() {
  console.log("======= LOG 1");
  try {
    // avec await d'une promise, pas de callback : tout se passe comme si la requête était synchrone :
    const raw_response = await https_request_with_async_await('https://api.nasa.gov/planetary/apod?api_key=DEMO_KEY');
    const response = JSON.parse(raw_response);
    console.log(response["explanation"]);
  } catch (error) {
    console.error(error);
  }

  console.log("======= LOG 2 (ce log est appelé *après* le lancement de la requête HTTP, pour vérifier si l'exécution est (a)synchrone)");

  const NOTES_SUR_LA_POC = `
  CONSTATÉ :
    - en awaitant une Promise, on gagne la possibilité d'ATTENDRE la résolution de la promise avant de poursuivre
    - (visible avec le log2 : il est affiché APRÈS résolution de la promise)
    - la POC montre qu'on peut wrapper une fonction qui attend une callback dans une Promise, pour pouvoir utiliser async/await dessus
  `;

  console.log("");
  console.log(NOTES_SUR_LA_POC);
}

main();


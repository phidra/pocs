import * as https from 'node:https';
import * as http from 'node:http';


class Not200Error extends Error {
  constructor(message: string) {
    super(message);
    this.name = "Not200Error";
  }
}

async function https_request_with_async_await(url: string): Promise<string> {
  return new Promise((resolve, reject) => {
    https.get(url, (response: http.IncomingMessage) => {
      let data = '';

      response.on('data', (chunk: string) => {
        data += chunk;
      });

      response.on('end', () => {
        if (response.statusCode === undefined || response.statusCode != 200) {
          const status_code_as_string = response.statusCode === undefined ? "undefined" : response.statusCode.toString();
          reject(new Not200Error(status_code_as_string));
        }
        resolve(data);
      });

    }).on("error", (error: Error) => {
      reject(new Error(error.message));
    });
  });
}


async function do_request(url: string): Promise<void> {
  console.log("======= LOG 1");
  try {
    // avec await d'une promise, pas de callback : tout se passe comme si la requête était synchrone :
    const raw_response = await https_request_with_async_await(url);
    const response = JSON.parse(raw_response);
    console.log(response["explanation"]);
  } catch (error) {
    if (error instanceof Not200Error) {
      console.error("STATUS CODE ERROR : ", error);
    } else {
      console.error("OTHER ERROR : ", error);
    }
  }
  console.log("======= LOG 2 (ce log est appelé *après* le lancement de la requête HTTP, pour vérifier si l'exécution est (a)synchrone)");
}

async function main() {
    const url_ok = 'https://api.nasa.gov/planetary/apod?api_key=DEMO_KEY';
    await do_request(url_ok);

    const url_ko_1_invalid_host = 'https://this-is-an-inexisting-host.fr/planetary/apod?api_key=DEMO_KEY';
    await do_request(url_ko_1_invalid_host);

    const url_ko_2_statuscode_404 = 'https://api.nasa.gov/planetary/this-is-a-valid-host-but-invalid-resource';
    await do_request(url_ko_2_statuscode_404);

  const NOTES_SUR_LA_POC = `
  CONSTATÉ :
    - comme dans la POC 2, la bonne façon de gérer les erreurs est de reject la promise
    - (ça permet d'afficher le log2 systématiquement, après échec de la promise)
  `;

  console.log("");
  console.log(NOTES_SUR_LA_POC);
}

main();


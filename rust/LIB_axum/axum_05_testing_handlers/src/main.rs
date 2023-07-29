use axum::{
    routing::get,
    Router,
    response::{Html, Response},
    Json,
    extract::{Path, State},
    response::IntoResponse,
    http::StatusCode,
};
use serde::Serialize;
use std::collections::HashMap;

const POC_DESCRIPTION: &str = "

REPRISE = maintenant que j'ai implémenté le fonctionnel de mon serveur, faire le testing à proprement parler. Références :
    https://github.com/tokio-rs/axum/blob/main/examples/testing/src/main.rs
    https://crates.io/crates/axum-test-helper
    https://crates.io/crates/axum-test

STILL TO DO :
    j'ai plein de clone, dont certains certainements évitables (e.g. ceux sur first_name / age en cas d'erreur)
    Dans une future autre POC, plutôt que de gérer les personnes dans une hashmap passée en state, le state ne contiendra qu'un 'person_finder' qui sera une interface.
        (et on pourra lui injecter un person_finder différent pour les tests d'intégration et le vrai serveur)
        cet exemple fait exactement ce que je veux = https://github.com/tokio-rs/axum/blob/main/examples/error-handling-and-dependency-injection/src/main.rs
        (et le serveur pourra même être une lib, avec une fonction différente pour créer le serveur (et ses handlers) et le lanceur)

POC sur le fait de tester les handlers axum.

La POC lance un server → le requête avec un browser (chaque page indique les URLs utiles)

Le server implémenté permet de retrouver les infos d'une personne à partir de son prénom et de son âge :

- 200 + réponse json si tout va bien
- ERROR 400 si erreur de parsing des paramètres
- ERROR 404 si URL inconnue
- ERROR 404 si URL valide, mais la personne recherchée n'est pas dans la base.

L'objectif de la POC est que chacune de ces situations soit testée.

";

const FOOTER: &str = r#"
<hr/>
<p> You may want to check those URLs :
    <ul>
        <li> <a href="/">root handler</a> </li>
        <li> <a href="/persons/Luke/32/">Luke</a> </li>
        <li> <a href="/persons/Kit/42/">Kit</a> </li>
        <li> <a href="/persons/wrong/parameter/">ERROR : invalid URL because age is not an int (should return 400) !</a> </li>
        <li> <a href="/persons/Inexisting/999/"> ERROR : valid URL, but inexisting person (should return 404) </a> </li>
        <li> <a href="/inexisting-url"> ERROR : invalid URL, because inexisting URL (should return 404) </a> </li>
    </ul>
</p>
"#;


#[derive(Serialize, Debug, Clone)]
struct Person {
    first_name: String,
    last_name: String,
    job: String,
    age: u32,
    address_number: i32,
    street: String,
    city: String,
    country: String,
}

async fn root_handler() -> Html<String> {
    Html(format!("
    <h1> Root handler </h1>
    {FOOTER}
    "))
}

async fn persons_handler(State(state): State<AppState>, Path((first_name, age)): Path<(String, u32)>) -> Response {
    let found = state.persons.get(&(first_name.clone(), age.clone()));
    match found {
        None => (StatusCode::NOT_FOUND, format!("person '{first_name} {age}' is unknown !")).into_response(),
        Some(person) => (StatusCode::OK, Json(person.clone())).into_response(),
    }
}


async fn fallback_404_handler() -> impl IntoResponse {
    eprintln!("ERROR : 404");
    let body = format!("
    <h1>404 handler </h1>
    <p style=\"color: red; font-weight: bold;\"> Ooops, this URL does not exist ! </p>
    {FOOTER}
    ");

    (StatusCode::NOT_FOUND, Html(body))
}

#[derive(Clone)]
struct AppState {
    persons: HashMap<(String, u32), Person>,
}

impl AppState {
    fn new() -> Self {
        let mut persons: HashMap<(String, u32), Person> = HashMap::new();
        persons.insert(
            (String::from("Luke"), 32),
            Person {
                    first_name: String::from("Luke"),
                    last_name: String::from("SKYWALKER"),
                    job: String::from("jedi"),
                    age: 32,
                    address_number: 10,
                    street: String::from("Sand Street"),
                    city: String::from("Sand-City"),
                    country: String::from("Tatooine"),
                }
        );
        persons.insert(
            (String::from("Kit"), 42),
            Person {
                    first_name: String::from("Kit"),
                    last_name: String::from("FISTO"),
                    job: String::from("jedi"),
                    age: 42,
                    address_number: 1,
                    street: String::from("Main Street"),
                    city: String::from("Palace"),
                    country: String::from("Coruscant"),
                }
        );
        AppState{persons}
    }
}

#[tokio::main]
async fn main() {
    println!("{POC_DESCRIPTION}");

    let state = AppState::new();

    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))
        .route("/persons/:first_name/:age/", get(persons_handler))
        .with_state(state)
    ;

    // adding a generic 404 handler :
    let app = app.fallback(fallback_404_handler);

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

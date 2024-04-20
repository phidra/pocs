use axum::{
    Json,
    routing::get,
    Router,
    response::Html,
};
use serde::{Serialize, Deserialize};
use std::collections::HashMap;

const POC_DESCRIPTION: &str = "

POC sur le fait de renvoyer une réponse JSON avec axum.

La POC lance un server → le requête avec un browser (chaque page indique les URLs utiles)

Le content-type renvoyé en sortie dépend du type de la réponse :
    https://docs.rs/axum/latest/axum/response/index.html#building-responses

";

const FOOTER: &str = r#"
<hr/>
<p> You may want to check those URLs :
<ul>
    <li> <a href="/">root handler</a> </li>
    <li> <a href="/json-data">handler that returns json</a> </li>
</ul>

<hr/>

<p>
La POC montre comment renvoyer des data json à partir d'une structure custom. Voici ce qu'en dit <a href="https://docs.rs/axum/latest/axum/response/index.html#building-responses"> la doc </a> :

<blockquote>
// `Json` will get a `application/json` content-type and work with anything that implements `serde::Serialize` <br/>
async fn json() -> Json<Vec<String>> { <br/>
    Json(vec!["foo".to_owned(), "bar".to_owned()]) <br/>
} <br/>
</blockquote>

</p>


"#;

async fn root_handler() -> Html<String>{
    Html(format!("
    <h1> Root handler </h1>
    <p> This is the root handler, nothing particular </p>
    {FOOTER}
    "))
}

async fn json_handler() -> Json<Person>{
    let person = Person {
        name: String::from("Luke SKYWALKER"),
        age: 32,
        friends: vec![
            Person{
                name: String::from("Leia SKYWALKER"),
                age: 32,
                friends: Vec::new(),
                cv: HashMap::new(),  // pour simplifier
            },
            Person{
                name: String::from("Obi-wan KENOBI"),
                age: 64,
                friends: Vec::new(),
                cv: HashMap::new(),  // pour simplifier
            },
        ],
        cv: HashMap::from([
            (Company::from("Jedi Academy"), NumberOfYearsInACompany(18)),
            (Company::from("Darth Sidious' right hand"), NumberOfYearsInACompany(10)),
            (Company::from("Death Star supervisor"), NumberOfYearsInACompany(1)),
        ]),
    };
    Json(person)
}

#[derive(Debug, Hash, PartialEq, Eq, Serialize, Deserialize)]
struct Company(String);
impl From<&str> for Company {
    fn from(val: &str) -> Company {
        Company(String::from(val))
    }
}
#[derive(Debug, Serialize, Deserialize)]
struct NumberOfYearsInACompany(u32);

#[derive(Serialize, Deserialize, Debug)]
struct Person {
    name: String,
    age: i32,
    friends: Vec<Person>,
    cv: HashMap<Company, NumberOfYearsInACompany>
}


#[tokio::main]
async fn main() {
    println!("{POC_DESCRIPTION}");

    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))
        .route("/json-data", get(json_handler))
    ;

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

use axum::{
    Json,
    routing::get,
    Router,
    response::IntoResponse,
    response::Response,
    response::Html,
};
use serde::{Serialize, Deserialize};

const POC_DESCRIPTION: &str = "

POC sur le fait qu'un même handler axum puisse renvoyer deux objets JSON différents.

Exemple d'utilisation = en succès, on renvoie un Json<SuccessResponse>, et en échec, on renvoie Json<FailureResponse>

La POC lance un server → le requêter avec un browser.

";

async fn root_handler() -> Html<String>{
    Html(format!(r#"
    <h1> Root handler </h1>
    <p> This is the root handler, nothing particular </p>
    <hr/>
    <p> You may want to check those URLs :
    <ul>
        <li> <a href="/">root handler</a> </li>
        <li> <a href="/json-success">handler that always returns successful json</a> </li>
        <li> <a href="/json-failure">handler that always returns failure json</a> </li>
        <li> <a href="/json-random">handler that randomly returns success or failure json</a> </li>
    </ul>
    "#))
}

async fn json_success_handler() -> Json<SuccessResponse> {
    let response = SuccessResponse {
        first_name: String::from("Luke"),
        last_name: String::from("SKYWALKER"),
        age: 32,
        description: String::from("A friendly jedi"),
    };
    Json(response)
}

async fn json_failure_handler() -> Json<FailureResponse> {
    let response = FailureResponse {
        error_message: String::from("You encountered an error because... I decided so !"),
        status_code: 500,
    };
    Json(response)
}

async fn json_random_handler() -> Response {
    if rand::random() {
        let response = SuccessResponse {
            first_name: String::from("Luke"),
            last_name: String::from("SKYWALKER"),
            age: 32,
            description: String::from("A friendly jedi"),
        };
        Json(response).into_response()
    } else {
        let response = FailureResponse {
            error_message: String::from("This is an alternative response..."),
            status_code: 999,
        };
        Json(response).into_response()
    }
}

#[derive(Serialize, Deserialize, Debug)]
struct SuccessResponse {
    first_name: String,
    last_name: String,
    age: i32,
    description: String,
}

#[derive(Serialize, Deserialize, Debug)]
struct FailureResponse {
    status_code: i32,
    error_message: String,
}


#[tokio::main]
async fn main() {
    println!("{POC_DESCRIPTION}");

    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))
        .route("/json-success", get(json_success_handler))
        .route("/json-failure", get(json_failure_handler))
        .route("/json-random", get(json_random_handler))
    ;

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

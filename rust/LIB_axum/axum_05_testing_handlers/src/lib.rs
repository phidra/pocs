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


#[derive(Serialize, Debug, Clone)]
struct Person {
    first_name: String,
    last_name: String,
    age: u32,
}

async fn root_handler() -> Html<String> {
    Html(format!(r#"
    <h1> Root handler </h1>
    <hr/>
    <p> You may want to check those URLs :
        <ul>
            <li> <a href="/">root handler</a> </li>
            <li> <a href="/persons/32/">Luke</a> </li>
            <li> <a href="/persons/42/">Kit</a> </li>
            <li> <a href="/persons/wrong-parameter/">ERROR : invalid URL because age is not an int (should return 400) !</a> </li>
            <li> <a href="/persons/999/"> ERROR : valid URL, but inexisting person (should return 404) </a> </li>
            <li> <a href="/inexisting-url"> ERROR : invalid URL, because inexisting URL (should return 404) </a> </li>
        </ul>
    </p>
    "#))
}

async fn persons_handler(State(state): State<AppState>, Path(age): Path<u32>) -> Response {
    let found = state.persons.get(&age);
    match found {
        None => (StatusCode::NOT_FOUND, format!("person with age '{age}' is unknown !")).into_response(),
        Some(person) => (StatusCode::OK, Json(person)).into_response(),
    }
}


async fn fallback_404_handler() -> impl IntoResponse {
    eprintln!("ERROR : 404");
    let body = format!("
        <h1>404 handler </h1>
        <p style=\"color: red; font-weight: bold;\"> Ooops, this URL does not exist ! </p>
    ");
    (StatusCode::NOT_FOUND, Html(body))
}

#[derive(Clone)]
struct AppState {
    persons: HashMap<u32, Person>,
}

impl AppState {
    fn new() -> Self {
        let mut persons: HashMap<u32, Person> = HashMap::new();
        let age = 32;
        persons.insert(
            age,
            Person {
                    first_name: String::from("Luke"),
                    last_name: String::from("SKYWALKER"),
                    age,
                }
        );

        let age = 42;
        persons.insert(
            age,
            Person {
                    first_name: String::from("Kit"),
                    last_name: String::from("FISTO"),
                    age,
                }
        );
        AppState{persons}
    }
}

pub fn create_server() -> Router {
    let state = AppState::new();

    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))
        .route("/persons/:age/", get(persons_handler))
        .with_state(state)
    ;

    // adding a generic 404 handler :
    let app = app.fallback(fallback_404_handler);

    app
}


#[cfg(test)]
mod tests {
    use super::*;
    use axum::{
        body::Body,
        http::{Request, StatusCode},
    };
    use tower::ServiceExt; // for `oneshot` and `ready`
    use serde_json::{json, Value};

    #[tokio::test]
    async fn valid_request_should_give_200() {
        let app = create_server();
        let response = app
            .oneshot(Request::builder().uri("/persons/32/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::OK);
    }

    #[tokio::test]
    async fn inexisting_person_should_give_404() {
        let app = create_server();
        let response = app
            .oneshot(Request::builder().uri("/persons/999/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::NOT_FOUND);
    }

    #[tokio::test]
    async fn invalid_path_parameter_should_give_400() {
        let app = create_server();
        let response = app
            .oneshot(Request::builder().uri("/persons/invalid-parameter/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::BAD_REQUEST);
    }

    #[tokio::test]
    async fn inexisting_url_should_give_404() {
        let app = create_server();
        let response = app
            .oneshot(Request::builder().uri("/inexisting-url").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::NOT_FOUND);
    }


    #[tokio::test]
    async fn body_is_valid_person_json() {
        let app = create_server();
        let response = app
            .oneshot(Request::builder().uri("/persons/32/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::OK);

        let body = hyper::body::to_bytes(response.into_body()).await.unwrap();
        let body: Value = serde_json::from_slice(&body).unwrap();
        assert_eq!(body, json!({
            "age": 32,
            "first_name": "Luke",
            "last_name": "SKYWALKER",
        }));
    }
}

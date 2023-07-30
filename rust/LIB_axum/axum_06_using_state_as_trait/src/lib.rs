use axum::{
    routing::get,
    Router,
    response::{Html, Response},
    Json,
    extract::{Path, State},
    response::IntoResponse,
    http::StatusCode,
};
use std::sync::Arc;
use serde::Serialize;
use std::collections::HashMap;


#[derive(Serialize, Debug, Clone)]
pub struct Person {
    first_name: String,
    last_name: String,
    age: u32,
}

pub struct PersonDatabaseError;

pub trait PersonDatabase {
    /// Loop up a person by their age.
    fn find(&self, age: u32) -> Result<&Person, PersonDatabaseError>;
}

pub type DynPersonDatabase = Arc<dyn PersonDatabase + Send + Sync>;


#[derive(Clone)]
pub struct ProductionPersonDatabase {
    persons: HashMap<u32, Person>,
}

impl ProductionPersonDatabase {
    pub fn new() -> Self {
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
        ProductionPersonDatabase{persons}
    }
}

impl PersonDatabase for ProductionPersonDatabase {
    fn find(&self, age: u32) -> Result<&Person, PersonDatabaseError> {
        println!("REQUEST> searching in database for person with age {}", &age);
        match self.persons.get(&age) {
            Some(person) => Ok(person),
            None => Err(PersonDatabaseError),
        }
    }
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

async fn persons_handler(State(persons): State<DynPersonDatabase>, Path(age): Path<u32>) -> Response {
    let found = persons.find(age);
    match found {
        Ok(person) => (StatusCode::OK, Json(person)).into_response(),
        Err(_) => (StatusCode::NOT_FOUND, format!("person with age '{age}' is unknown !")).into_response(),
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


pub fn create_server(persons: DynPersonDatabase) -> Router {
    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))
        .route("/persons/:age/", get(persons_handler))
        .with_state(persons)
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


    #[derive(Clone)]
    struct StubPersonDatabase {
        stub_person: Person,
    }
    impl StubPersonDatabase {
        pub fn new() -> Self {
            StubPersonDatabase {
                stub_person: Person {
                    first_name: String::from("Toto"),
                    last_name: String::from("TATA"),
                    age: 42,
                }
            }
        }
    }
    impl PersonDatabase for StubPersonDatabase {
        fn find(&self, _age: u32) -> Result<&Person, PersonDatabaseError> {
            Ok(&self.stub_person)
        }
    }


    #[derive(Clone)]
    struct InexistingPersonDatabase;
    impl PersonDatabase for InexistingPersonDatabase {
        fn find(&self, _age: u32) -> Result<&Person, PersonDatabaseError> {
            Err(PersonDatabaseError)
        }
    }

    #[tokio::test]
    async fn valid_request_should_give_200() {
        let persons = Arc::new(StubPersonDatabase::new()) as DynPersonDatabase;
        let app = create_server(persons);
        let response = app
            .oneshot(Request::builder().uri("/persons/32/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::OK);
    }

    #[tokio::test]
    async fn inexisting_person_should_give_404() {
        let persons = Arc::new(InexistingPersonDatabase) as DynPersonDatabase;
        let app = create_server(persons);
        let response = app
            .oneshot(Request::builder().uri("/persons/999/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::NOT_FOUND);
    }

    #[tokio::test]
    async fn invalid_path_parameter_should_give_400() {
        let persons = Arc::new(StubPersonDatabase::new()) as DynPersonDatabase;
        let app = create_server(persons);
        let response = app
            .oneshot(Request::builder().uri("/persons/invalid-parameter/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::BAD_REQUEST);
    }

    #[tokio::test]
    async fn inexisting_url_should_give_404() {
        let persons = Arc::new(StubPersonDatabase::new()) as DynPersonDatabase;
        let app = create_server(persons);
        let response = app
            .oneshot(Request::builder().uri("/inexisting-url").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::NOT_FOUND);
    }


    #[tokio::test]
    async fn body_is_valid_person_json() {
        let persons = Arc::new(StubPersonDatabase::new()) as DynPersonDatabase;
        let app = create_server(persons);
        let response = app
            .oneshot(Request::builder().uri("/persons/5/").body(Body::empty()).unwrap())
            .await
            .unwrap();
        assert_eq!(response.status(), StatusCode::OK);

        let body = hyper::body::to_bytes(response.into_body()).await.unwrap();
        let body: Value = serde_json::from_slice(&body).unwrap();
        assert_eq!(body, json!({
            "age": 42,
            "first_name": "Toto",
            "last_name": "TATA",
        }));
    }
}

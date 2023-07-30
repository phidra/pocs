use poc_axum::{create_server, DynPersonDatabase, ProductionPersonDatabase};
use std::sync::Arc;

const POC_DESCRIPTION: &str = r#"

Cette POC est une amélioration de la précédente (qui testait unitairement les handlers axum) :
    - la vraie POC est `cargo test`, puisqu'il s'agit de démontrer le testing unitaire des handlers HTTP.
    - mais on peut aussi lancer un vrai serveur avec les handlers testé avec `cargo run`, auquel cas l'URL racine indique les URLs à requêter.

Pour rappel, la POC précédent avait un handler permettant de retrouver les infos d'une personne à partir de son âge.
L'amélioration consiste à injecter la database des personnes sous forme d'une interface (un trait).
On y gagne le fait de pouvoir utiliser une database différente pour le vrai serveur et pour les tests.

Ici, la fonction qui crée le serveur accepte par injection de dépendance un truc qui se conforme au trait PersonDatabase.
Du coup, les tests unitaires utilisent des databases DIFFÉRENTES de celle utilisée en "production".

(note : il faut en réalité passer un DynPersonDatabase et l'appelant doit donc construire lui-même l'Arc<PersonDatabase> à passer,
je n'arrive pas encore à pouvoir passer un PersonDatabase, avec create_server qui crée lui-même l'Arc, mais je suis sûr que c'est possible)

Cette POC est dérivée de :
    https://github.com/tokio-rs/axum/blob/main/examples/testing/src/main.rs
    https://github.com/tokio-rs/axum/blob/main/examples/error-handling-and-dependency-injection/src/main.rs
"#;

#[tokio::main]
async fn main() {
    println!("{POC_DESCRIPTION}");

    let persons = Arc::new(ProductionPersonDatabase::new()) as DynPersonDatabase;
    let app = create_server(persons);

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

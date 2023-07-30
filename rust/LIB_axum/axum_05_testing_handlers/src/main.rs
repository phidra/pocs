use poc_axum::create_server;

const POC_DESCRIPTION: &str = "

POC sur le fait de tester les réponses des handlers axum de façon unitaire (i.e. sans avoir à lancer de serveur HTTP) :
    - la vraie POC est `cargo test`, puisqu'il s'agit de démontrer qu'on peut tester les handlers HTTP sans avoir à lancer un vrai serveur HTTP.
    - mais on peut aussi lancer un vrai serveur avec les handlers testé avec `cargo run`, auquel cas l'URL racine indique les URLs à requêter.


Cette POC est dérivée de :
    https://github.com/tokio-rs/axum/blob/main/examples/testing/src/main.rs


Les handlers implémentés permettent de retrouver les infos d'une personne à partir de son âge. Ils renvoient différents status :
    - OK 200 + réponse json si tout va bien
    - ERROR 400 si erreur de parsing des paramètres
    - ERROR 404 si URL inconnue
    - ERROR 404 si URL valide, mais la personne recherchée n'est pas dans la base.
L'objectif de la POC est que chacune de ces situations soit testée, et ce sans avoir à lancer réellement le serveur HTTP.


Dans la présente POC, la database des personnes connues est une hashmap partagée entre tous les handlers par un axum::extract::State.
Dans une future autre POC, le state ne contiendra qu'une interface 'person_finder' (un trait) plutôt qu'une implémentation concrète, cf. 
    https://github.com/tokio-rs/axum/blob/main/examples/error-handling-and-dependency-injection/src/main.rs
(et on pourra lui injecter un person_finder différent pour les tests et pour le vrai serveur)

";

#[tokio::main]
async fn main() {
    println!("{POC_DESCRIPTION}");

    let app = create_server();

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

use axum::{
    routing::get,
    extract::Path,
    extract::rejection::PathRejection,
    Router,
    response::Html,
    response::IntoResponse,
    http::StatusCode,
};

const FOOTER: &str = "
<hr/>
<p> You may want to check those URLs :
<ul>
<li> <a href=\"/\">root handler</a> </li>
<li> <a href=\"/inexisting-url\">what happen if we request an inexisting URL ?</a> (spoiler alert : will return a 404 and also return a valid body describing the error) </li>
<li> <a href=\"/one-path-param/42\">one valid path param</a> </li>
<li> <a href=\"/one-path-param-with-custom-error-handling/43\">another valid path param</a> </li>
<li> <a href=\"/one-path-param/99999\">one invalid path param (parameter is too big)</a>, will return a 400 </li>
<li> <a href=\"/one-path-param/pouet\">one invalid (passing string instead of int) path param</a>, will return a 400, with <b>AN AXUM BODY </b></li>
<li> <a href=\"/one-path-param-with-custom-error-handling/pouet\">one invalid (passing string instead of int) path param</a>, will return a 400, with <b> A CUSTOM BODY</b> </li>
<li> <a href=\"/one-path-param-with-custom-error-handling\">one invalid (missing) path param</a>, will return a 404 </li>
<li> <a href=\"/path-that-returns-500\">url that always returns 500</a> </li>

</ul>

<p> Try also with <code>curl -v MYURL</code> to see the status-code </p>

<p>
Axum' status codes are documented <a href=\"https://docs.rs/http/0.2.9/http/status/struct.StatusCode.html\">here</a>.
</p>

<hr/>

<p>
La POC contient divers trucs utiles :

<ul>
<li> Comment retourner des status-code invalides (avec un body human-readable). </li>
<li> Comment gérer les 404 (avec un body human-readable) </li>
<li> Comment gérer les erreurs de parsing de paramètres (avec un body human-readable) </li>
</ul>

Elle ignore les query-params, mais le principe serait le même.
</p>
";

async fn root_handler() -> Html<String>{
    Html(format!("
    <h1> Root handler </h1>
    <p> This is the root handler, nothing particular </p>
    {FOOTER}
    "))
}


fn process_integer_between_1_and_100_request(param: u32) -> (StatusCode, Html<String>) {
    let mut result = String::from("
    <h1> One param handler </h1>
    <p> This handler handles params both in path and in queries </p>
    ");

    // en revanche, il sera appelé si on passe un entier invalide
    const LOWER_BOUND: u32 = 1;
    const UPPER_BOUND: u32 = 100;
    if param < LOWER_BOUND || param > UPPER_BOUND {
        let error_string = format!("the parameter {param} is not between bounds [{LOWER_BOUND} -> {UPPER_BOUND}] !");
        eprintln!("ERROR : {error_string}");
        result.push_str(&format!("<p style=\"color: red; font-weight: bold;\"> Ooops, {error_string} </p>"));
        result.push_str(&FOOTER);
        return (StatusCode::BAD_REQUEST, Html(result));
    }

    result.push_str(&format!("<p> The path param is a valid integer and is : <b><code>{param}</code></b> </p>"));
    result.push_str(&FOOTER);
    (StatusCode::OK, Html(result))
}


async fn integer_between_1_and_100_param_handler(Path(param): Path<u32>) -> impl IntoResponse {
    // ce handler ne sera PAS appelé si on n'a pas pu parser le path-param en int
    // l'erreur aura lieu en amont, et retournera une 400 à l'utilisateur sans passer ici
    // (car ce handler attend un Path<u32>, et non un Result<Path<u32>>)
    process_integer_between_1_and_100_request(param)
}


async fn maybe_integer_between_1_and_100_param_handler(param: Result<Path<u32>, PathRejection>) -> impl IntoResponse {
    // see https://docs.rs/axum/latest/axum/extract/rejection/enum.PathRejection.html
    match param {
        Ok(Path(param)) => process_integer_between_1_and_100_request(param),
        Err(PathRejection::FailedToDeserializePathParams(deserialization_failure)) => (
                StatusCode::BAD_REQUEST,
                Html(format!("
                    <h1>Custom deserialization failure handler </h1>
                    <p style=\"color: red; font-weight: bold;\"> Ooops, couldn't deserialize param : {deserialization_failure} ! </p>
                    {FOOTER}
                "))
            ),
        // NOTE : on dirait qu'on obtient plutôt une 404 quand il manque un path parameter (ce qui est logique),
        // du coup je ne comprends pas bien dans quel cas on passerait ici...
        Err(PathRejection::MissingPathParams(missing_param)) => {(
            StatusCode::BAD_REQUEST,
            Html(format!("
                <h1>Custom deserialization failure handler </h1>
                <p style=\"color: red; font-weight: bold;\"> Ooops, there is a missing param : {missing_param} ! </p>
                {FOOTER}
            ")),
        )},
        Err(_) => {(
            StatusCode::INTERNAL_SERVER_ERROR,
            Html(format!("
                <h1>500 handler </h1>
                <p style=\"color: red; font-weight: bold;\"> Ooops, this URL created an internal error ! </p>
                <p> THIS IS UNEXPECTED ! </p>

                {FOOTER}
            ")),
        )}
    }
}



async fn return_500_handler() -> impl IntoResponse {
    eprintln!("ERROR (on purpose) : this handler always return 500");
    let body = format!("
    <h1>500 handler </h1>
    <p style=\"color: red; font-weight: bold;\"> Ooops, this URL created an internal error ! </p>
    <p> Worry not : this is intentional ;-) </p>

    {FOOTER}
    ");

    // apparement, il suffit de retourner une paire (status, Html(body)) pour faire une réponse :
    (StatusCode::INTERNAL_SERVER_ERROR, Html(body))
}


async fn generic_404_handler() -> impl IntoResponse {
    eprintln!("ERROR : 404");
    let body = format!("
    <h1>Generic 404 handler </h1>
    <p style=\"color: red; font-weight: bold;\"> Ooops, this URL does not exist ! </p>
    {FOOTER}
    ");

    // apparement, il suffit de retourner une paire (status, Html(body)) pour faire une réponse :
    (StatusCode::NOT_FOUND, Html(body))
}

#[tokio::main]
async fn main() {
    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))

        // ce handler prend un Path<u32> → si le parsing du paramètre échoue, on ne passera pas dedans :
        .route("/one-path-param/:param", get(integer_between_1_and_100_param_handler))

        // ce handler prend un Result<Path<u32>> → on passera dedans même si le parsing échoue :
        .route("/one-path-param-with-custom-error-handling/:param", get(maybe_integer_between_1_and_100_param_handler))

        .route("/path-that-returns-500", get(return_500_handler));

    // adding a generic 404 handler :
    let app = app.fallback(generic_404_handler);

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

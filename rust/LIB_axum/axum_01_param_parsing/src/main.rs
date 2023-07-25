use axum::{
    routing::get,
    extract::Path,
    extract::Query,
    Router,
    response::Html,
};
use std::collections::HashMap;

const FOOTER: &str = "
<hr/>
<p> You may want to check those URLs :
<ul>
<li> <a href=\"/\">root handler</a> </li>
<li> <a href=\"/no-path-param\">no path params, no query params </a> </li>
<li> <a href=\"/no-path-param?luke=skywalker&leia=skywalker&han=solo\">no path params but query params </a> </li>
<li> <a href=\"/one-path-param/avocado/end\">one path param but no query params</a> </li>
<li> <a href=\"/one-path-param/banana/end?luke=skywalker&leia=skywalker&han=solo\">both one path param AND query params</a> </li>
<li> <a href=\"/two-path-params/rox/inbetween/rouky/end?luke=skywalker&leia=skywalker&han=solo\">two path params AND query params</a> </li>
</ul>
";

async fn root_handler() -> Html<String>{
    Html(format!("
    <h1> Root handler </h1>
    <p> This is the root handler, nothing particular </p>
    {FOOTER}
    "))
}

fn query_params_to_html_string(query_params: &HashMap<String, String>) -> String {
    if query_params.is_empty() {
        return String::from("<p> There are no query params ! </p>");
    }

    let mut result = String::from("<p> Here are the query params : </p>");
    result.push_str("<ul>");
    for (key, value) in query_params {
        result.push_str(&format!("<li> {key}={value} </li>"));
    }
    result.push_str("</ul>");
    result
}

async fn no_path_param_handler(Query(query_params): Query<HashMap<String, String>>) -> Html<String> {
    let mut result = String::from("
    <h1> No param handler </h1>
    <p> This handler handles params in queries only </p>
    <p> There are no path params ! </p>
    ");
    result.push_str(&query_params_to_html_string(&query_params));
    result.push_str(&FOOTER);
    Html(result)
}

async fn one_path_param_handler(Path(param): Path<String>, Query(query_params): Query<HashMap<String, String>>) -> Html<String> {
    let mut result = String::from("
    <h1> One param handler </h1>
    <p> This handler handles params both in path and in queries </p>
    ");
    result.push_str(&format!("<p> The path param is : <b><code>{param}</code></b> </p>"));
    result.push_str(&query_params_to_html_string(&query_params));
    result.push_str(&FOOTER);
    Html(result)
}

async fn two_path_params_handler(Path((param1, param2)): Path<(String, String)>, Query(query_params): Query<HashMap<String, String>>) -> Html<String> {
    let mut result = String::from("
    <h1> Two param handler </h1>
    <p> This handler handles params both in path and in queries </p>
    ");
    result.push_str(&format!("<p> The path params are : <b><code>{param1}</code></b>  AND  <b><code>{param2}</code></b> </p>"));
    result.push_str(&query_params_to_html_string(&query_params));
    result.push_str(&FOOTER);
    Html(result)
}

#[tokio::main]
async fn main() {
    // build our application with a single route
    let app = Router::new()
        .route("/", get(root_handler))
        .route("/no-path-param", get(no_path_param_handler))
        .route("/one-path-param/:param/end", get(one_path_param_handler))
        .route("/two-path-params/:param1/inbetween/:param2/end", get(two_path_params_handler));

    let url = "0.0.0.0:3000";
    println!("\nListening to requests on http://{url}");
    axum::Server::bind(&url.parse().unwrap())
        .serve(app.into_make_service())
        .await
        .unwrap();
}

const POC_DESCRIPTION: &str = r#"

POC sur les patterns réfutables et irréfutables.

En résumé, un pattern réfutable est un pattern qui a la possibilité de ne pas matcher au runtime.
Un pattern irréfutable matchera quoi qu'il arrive au runtime.

"#;


fn main() {
    println!("{POC_DESCRIPTION}");

    let maybe: Option<i32> = if rand::random() { Some(42) } else { None };
    // ^ ceci est une Option qui peut contenir Some ou None, ça ne sera décidé qu'au runtime


    // un let-statement attend un pattern irréfutable.
    // Or, le let-statement ci-dessus EST RÉFUTABLE !
    // En effet, si maybe vaut None, le pattern ne matchera pas.
    // Du coup, la ligne suivante ne compile pas :
    // let Some(y) = maybe;

    // à l'inverse, if let accepte les patterns réfutables, donc
    // la ligne suivante compile. C'est au runtime qu'on saura si
    // on passe dedans ou pas :
    if let Some(x) = maybe {
        println!("On a eu un Some, qui contenait : {}", x);
    }

    println!("(ceci est la fin de la POC... la partie intéresante est l'éventuelle ligne précédente)");
}

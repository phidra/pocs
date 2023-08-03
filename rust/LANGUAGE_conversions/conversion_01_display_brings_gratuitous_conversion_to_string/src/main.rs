use std::fmt;

const POC_DESCRIPTION: &str = r#"

POC sur deux choses liés au fait pour un type d'implémenter le trait std::fmt::Display :

1. implémenter Display permet gratuitement de convertir son type en string avec `x.to_string()`
2. implémenter Display permet `x.to_string()`, mais NE PERMET PAS `String::from(x)`
3. pour pouvoir faire `String::from(x)`, std::fmt::Display ne sert à rien, il faut implémenter `From<String> for MyType`

My guess, c'est que les intentions sont différentes :

- Display permet d'afficher (et il pourrait être acceptable d'afficher de façon identique toutes les instances possibles d'un type)
- String::from véhicule une sémantique de conversion : on s'attend à ce que deux instances différentes d'un type donnent des strings différentes

"#;


#[allow(dead_code)]
struct MyNonDisplayType;

struct MyDisplayType;
impl fmt::Display for MyDisplayType {
        fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "this is my super DisplayType")
    }
}

fn main() {
    println!("{POC_DESCRIPTION}");

    // on peut bien afficher le type qui implémente std::fmt::Display :
    let display = MyDisplayType;
    println!("{}", display.to_string());

    // mais on ne peut pas afficher non-display, ceci ne compilera pas :
    // let non_display = MyNonDisplayType;
    // println!("{}", non_display.to_string());

    // par ailleurs, on ne peut pas String::from(display) non plus !
    // ceci ne compilera pas :
    let my_string = String::from(display);
    println!("{}", my_string);

    // ... à moins d'implémenter le trait From<Type> for String (même si la déclaration est faite après l'utilisation) :
    // (commenter l'implémentation ci-dessous pour constater que String::from(display) ne compile plus)
    impl From<MyDisplayType> for String {
        fn from(_value: MyDisplayType) -> String {
            String::from("Look Mom, this string was created with String::from() !")
        }
    }
}

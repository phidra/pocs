use std::collections::HashMap;
use crate::person::Person;

pub type Persons = HashMap<String, Person>;

fn build_map() -> Persons {
    let mut persons: Persons = HashMap::new();
    persons.insert(
        String::from("Luke"),
        Person {
                first_name: String::from("Luke"),
                last_name: String::from("SKYWALKER"),
                age: 32,
            }
    );
    persons.insert(
        String::from("Kit"),
        Person {
                first_name: String::from("Kit"),
                last_name: String::from("FISTO"),
                age: 42,
            }
    );
    println!("the map has {} elements", persons.len());
    for (key, val) in &persons {
        println!("{key:?} = {:?}", val);
    }
    persons
}

pub fn get_where_keys_are_strings() {
    let persons = build_map();

    let luke = String::from("Luke");
    let found = persons.get(&luke).unwrap();

    // comme la clé de la hashmap est une string simple, j'ai pu requêter avec une &String ...
    // ...ce qui m'autorise à réutiliser la string derrière :
    println!("We just requested with the string : {luke}");
    found.display();

    // on peut aussi requêter avec un slice &str :
    let found = persons.get("Kit").unwrap();
    found.display();
}

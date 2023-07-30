use std::collections::HashMap;
use crate::person::Person;

pub type Persons = HashMap<(String, u32), Person>;

fn build_map() -> Persons {
    let mut persons: Persons = HashMap::new();
    persons.insert(
        (String::from("Luke"), 32),
        Person {
                first_name: String::from("Luke"),
                last_name: String::from("SKYWALKER"),
                age: 32,
            }
    );
    persons.insert(
        (String::from("Kit"), 42),
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

pub fn get_where_keys_are_tuples() {
    let persons = build_map();

    let luke = String::from("Luke");
    let pair = (luke, 32);
    let found = persons.get(&pair).unwrap();

    // Je n'arrive pas à réutiliser la string après requêting de la hashmap...
    // Décommenter cette ligne ne compilera pas :
    // println!("We just requested with the string : {luke}");

    found.display();
}

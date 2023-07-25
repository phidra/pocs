const POC_DESCRIPTION: &str = "

POC sur la serialization/deserialization en rust avec serde.

C'est un quasi-clone de l'exemple donné dans https://serde.rs/

https://serde.rs/#design

> Where many other languages rely on runtime reflection for serializing data, Serde is instead built on Rust's powerful trait system.
> A data structure that knows how to serialize and deserialize itself is one that implements Serde's Serialize and Deserialize traits
> (or uses Serde's derive attribute to automatically generate implementations at compile time).
> This avoids any overhead of reflection or runtime type information.

^ tout se passe au compile-time.

https://serde.rs/#data-formats

^ il y a une grosse liste de formats gérés, notamment json, yaml, toml

https://serde.rs/#data-structures

> Out of the box, Serde is able to serialize and deserialize common Rust data types (...)
> For example String, &str, usize, Vec<T>, HashMap<K,V> are all supported.
> Serde provides a derive macro to generate serialization implementations for structs in your own program.

^ les types usuels sont traités par défaut, et si mes structs n'ont que des champs classiques, je gagne la serialization gratos avec un simple #[derive]

";

use std::collections::HashMap;
use serde::{Serialize, Deserialize};

// see https://doc.rust-lang.org/rust-by-example/generics/new_types.html :
#[derive(Debug, Hash, PartialEq, Eq, Serialize, Deserialize)]
struct Name(String);
impl From<&str> for Name {
    fn from(val: &str) -> Name {
        Name(String::from(val))
    }
}

#[derive(Debug, Serialize, Deserialize)]
struct Description(String);
impl From<&str> for Description {
    fn from(val: &str) -> Description {
        Description(String::from(val))
    }
}

#[derive(Serialize, Deserialize, Debug)]
struct MyStruct {
    x: i32,
    y: i32,
    who: String,
    friends: HashMap<Name, Description>,
    foes: Vec<Name>,
}

impl MyStruct {
    fn new() -> MyStruct {
        MyStruct {
            x: 1,
            y: 2,
            who: String::from("Luke SKYWALKER"),
            friends: HashMap::from([
                (Name::from("Yoda"), Description::from("A small green goblin")),
                (Name::from("Leia"), Description::from("A beautiful wife")),
            ]),
            foes: vec![Name::from("His anger"), Name::from("His pride"), Name::from("Himself generally !")],
        }
    }
}


fn main() {
    println!("{POC_DESCRIPTION}");

    let myobject = MyStruct::new();

    let serialized = serde_json::to_string_pretty(&myobject).unwrap();
    println!("serialized = {}", serialized);

    let deserialized: MyStruct = serde_json::from_str(&serialized).unwrap();
    println!("deserialized = {:?}", deserialized);
}

const POC_DESCRIPTION: &str = r#"

L'objectif de cette POC est de creuser un souci que j'ai rencontré :

Si une HashMap a des String comme clés :

    - alors je peux requêter la hashmap avec des &String (et même des slices, mais c'est secondaire)
    - du coup, je ne consomme pas la String utilisée pour requêter

Mais si une HashMap a des (String, i32) comme clés :

    - alors je peux requêter la hashmap avec des &(String, i32), mais PAS avec des (&String, i32) ou &(&String, i32) !
    - du coup, je suis obligé de consommer la String utilisée pour requêter !


Note que la doc de HashMap indique bien que la clé peut être un borrow :

https://doc.rust-lang.org/stable/std/collections/struct.HashMap.html#method.get

 (et donc il est acceptable de passer une référence là où la clé est une valeur)
MAIS ici, la clé est une paire, et je veux une référence sur le premier élément de la pair.

À l'état actuel de mes connaissances rust, j'ai l'impression que je suis coincé, et que je ne peux éviter de cloner pour contruire la paire à requêter.

"#;

mod person;

mod keys_are_strings;
use keys_are_strings::get_where_keys_are_strings;

mod keys_are_tuples;
use keys_are_tuples::get_where_keys_are_tuples;

fn main() {
    println!("{POC_DESCRIPTION}");
    get_where_keys_are_strings();
    get_where_keys_are_tuples();
}

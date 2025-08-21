use std::rc::Rc;
use std::thread;

const POC_DESCRIPTION: &str = r#"

Cette POC montre que pour pouvoir passer une référence vers un objet à un thread détaché,
l'objet doit être d'un type `Sync`, et c'est le cas même pour passer une UNIQUE référence.

On pourrait penser qu'en passant une UNIQUE référence dans un thread, il n'y a en réalité
aucune concurrence, mais rust ne distingue pas ce cas particulier, et est un peu "trop"
conservateur.

Comme il y a équivalence enre "T est Sync" et "&T est Send" (d'après le rustonomicon),
pouvoir envoyer une référence vers un thread revient à dire que T est Sync :

    https://doc.rust-lang.org/nomicon/send-and-sync.html
    > T is Sync if and only if &T is Send

Indeed, l'erreur du compilo indique bien qu'on veut que T soit Sync pour envoyer un &T :

> within `NeitherSendNorSync`, the trait `Sync` is not implemented for `Rc<i32>`
> (...)
> note: required for `&NeitherSendNorSync` to implement `Send`

Pour la voir apparaître, décommenter le passage de NeitherSendNorSync vers un thread.

"#;

// TYPE 1 = Send et Sync :
#[allow(dead_code)]
struct SendAndSync {
    field: i32,
}
impl SendAndSync {
    fn get_string(&self) -> String {
        "SendAndSync !".to_string()
    }
}

// TYPE 2 = ni Send ni Sync :
#[allow(dead_code)]
struct NeitherSendNorSync {
    field: Rc<i32>, // juste pour que le type ne soit ni Send ni Sync
}
impl NeitherSendNorSync {
    fn get_string(&self) -> String {
        "NeitherSendNorSync !".to_string()
    }
}

fn main() {
    println!("{POC_DESCRIPTION}");

    // Avec un type Send et Sync, on peut :
    //    1. utiliser l'objet dans le thread principal
    //    2. puis envoyer une référence vers l'objet dans un thread
    //    3. puis utiliser l'objet de nouveau dans le thread principal
    let obj1 = SendAndSync { field: 99 };
    let obj1_ref = &obj1;
    println!("From main thread : {}", obj1_ref.get_string());
    thread::scope(|scope| {
        scope.spawn(|| {
            println!("From secondary thread : {}", obj1_ref.get_string());
        });
    });
    println!("From main thread again : {}", obj1_ref.get_string());

    // Avec un type qui n'est pas Sync (ni Send, mais ça ne joue pas), si on essaye
    // de faire la même chose, ça ne compile pas :
    let rc = Rc::new(42);
    let obj2 = NeitherSendNorSync { field: rc };
    let obj2_ref = &obj2;
    println!("From main thread : {}", obj2_ref.get_string());
    // DÉCOMPILER CE QUI SUIT POUR VOIR L'ERREUR DU COMPILATEUR :
    // thread::scope(|scope| {
    //     scope.spawn(|| {
    //         println!("From secondary thread : {}", obj2_ref.get_string());
    //     });
    // });
    println!("From main thread again : {}", obj2_ref.get_string());
}

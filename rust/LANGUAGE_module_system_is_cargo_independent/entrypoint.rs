mod sayer;

const POC_DESCRIPTION: &str = r#"

POC montrant que le module system de rust est propre à la compilation rustc (et non apportée par cargo).
La présente POC le montre en n'utilisant pas cargo pour builder le "main" (qui s'appelle ici entrypoint).
Pourtant, on utilise tout de même le système de modules.

Conceptuellement, le compilateur rust et C++ fonctionnent très différement :

- le compilo C++ fabrique des trucs intermédiaires (= des fichiers .o), puis le linker fabrique un truc final à partir de ça = un binaire, ou une lib.so / lib.a
- le compilo rust fait directement des "trucs finaux" : binaire ou lib.so


Du coup, chaque "unité de compilation" (= d'une crate) en rust correspond à un binaire final (ou lib.so) !

Le module-tree d'une crate est donc le module-tree de la lib.so finale

L'équivalent C++ des modules rust sont donc les "unités de compilation C++" = les fichiers cpp.

"#;


fn main() {
    println!("{POC_DESCRIPTION}");
    sayer::say_youpi();
    sayer::laugher::say_haha();
}

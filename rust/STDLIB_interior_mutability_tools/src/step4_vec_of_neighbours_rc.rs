const STEP_DESCRIPTION: &str = r#"

Cette step4 remplace les références par des Rc pour stocker les neighbours.

WORK IN PROGRESS

REPRISE = faire en sorte que chaque node stocke un Rc de Node

(montrer que ça règle une partie des soucis, mais que comme les Rc sont immutables, ils nous manque RefCell)
(puis, faire l'étape 5 = étape finale = on stocke chaque Neighbour comme Rc<RefCell<T>>)


"#;

pub fn poc() {
    println!("{STEP_DESCRIPTION}");
}

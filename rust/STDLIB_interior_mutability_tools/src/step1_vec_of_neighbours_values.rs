const STEP_DESCRIPTION: &str = r#"
Pour cette step 1, on implémente le graphe naïvement :

- un node est une simple paire (value, neighbours)
- les neighbours sont un Vector de values = les nodes

Cette implémentation ne fonctionne pas : dès qu'on veut ajouter un voisin V à un node N,
on doit move V dans N.neighbours, et V n'est plus utilisable directement (sans compter
qu'un node ne peut être voisin que d'un unique node.

"#;

// un node est paramétré sur une valeur T, et contient une liste de neighbours :
#[derive(Debug)]
#[allow(dead_code)]
struct Node<T> {
    value: T,
    neighbours: Vec<Node<T>>,
}

impl<T> Node<T> {
    /// creation with no neighbours :
    fn new(value: T) -> Self {
        Node {
            value,
            neighbours: vec![],
        }
    }

    fn add_neighbour(&mut self, node: Node<T>) {
        self.neighbours.push(node);
    }
}

pub fn poc() {
    println!("{STEP_DESCRIPTION}");
    let n1 = Node::new(42);
    let n2 = Node::new(43);
    println!("{:?}", n1);
    println!("{:?}", n2);

    let mut n3 = Node::new(44);
    n3.add_neighbour(n1);
    println!("{:?}", n3);

    // BOUM : le Node n1 n'existe plus en tant que tel : il a été moved dans n3.neighbours
    // Si on décommente ce qui suit, ça ne compile plus :
    // println!("{:?}", n1);
}

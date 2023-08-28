const STEP_DESCRIPTION: &str = r#"
Cette step2 montre qu'un vector de références vers les nodes ne marche pas beaucoup mieux.

Cette implémentation corrige un souci de la step1 (= on ne perd pas les nodes quand on
ajoute des voisins), mais ne fonctionne pas non plus : les voisins sont de facto immutables
(car on ne peut avoir qu'une unique référence mutable), et notamment, une fois qu'un node
est un voisin d'un autre, on ne peut plus le muter.

"#;

// un node est paramétré sur une valeur T, et contient une liste de neighbours :
#[derive(Debug)]
#[allow(dead_code)]
struct Node<'a, T> {
    value: T,
    neighbours: Vec<&'a Node<'a, T>>,
}

impl<'a, T> Node<'a, T> {
    /// creation with no neighbours :
    fn new(value: T) -> Self {
        Node {
            value,
            neighbours: vec![],
        }
    }

    fn add_neighbour(&mut self, node: &'a Node<'a, T>) {
        self.neighbours.push(node);
    }
}

pub fn poc() {
    println!("{STEP_DESCRIPTION}");
    #[allow(unused_mut)]
    let mut n1 = Node::new(42);
    let n2 = Node::new(43);
    println!("{:?}", n1);
    println!("{:?}", n2);

    let mut n3 = Node::new(44);
    n3.add_neighbour(&n1);
    println!("{:?}", n3);

    // Par rapport à la step1, on a mainenant toujours accès à un node même s'il est un voisin d'un autre node :
    println!("{:?}", n1);

    // Mais une fois qu'un node est un voisin, il est irrémédiablement immutable :
    // Si on décommente ce qui suit, ça ne compile plus :
    // n1.add_neighbour(&n2);

    println!("{:?}", n1);
    println!("{:?}", n2);
    println!("{:?}", n3);
}

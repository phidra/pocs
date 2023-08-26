const STEP_DESCRIPTION: &str = r#"

Cette step3 montre les défauts du design de la step3 :

- ici, j'essaye de stocker les nodes dans un graphe
- comme chaque node définit ses voisins avec un vector d'immutable-borrows...
- ...alors ça devient impossible de muter un node pour lui ajouter des voisins
- en effet, il faut à la fois avoir :
    - un mutable borrow (sur le node qu'on veut muter pour lui ajouter des voisins)
    - un immutable borrow (sur le node qui sera le voisin)
- et comme tous ces nodes appartiennent au même field du graphe, ça revient à nécessiter
  à la fois un borrow mutable et immutable, ce que le borrow-checker interdit.


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

    #[allow(dead_code)]
    fn add_neighbour(&mut self, node: &'a Node<'a, T>) {
        self.neighbours.push(node);
    }
}

#[derive(Debug)]
struct Graph<'a, T> {
    nodes: Vec<Node<'a, T>>,
}

impl<'a, T> Graph<'a, T> {
    fn new() -> Self {
        Graph { nodes: vec![] }
    }

    fn create_node(&mut self, val: T) {
        self.nodes.push(Node::new(val));
    }

    fn get_node(&'a self, idx: usize) -> Option<&'a Node<'a, T>> {
        self.nodes.get(idx)
    }

    #[allow(dead_code)]
    fn get_mut_node(&'a mut self, idx: usize) -> Option<&'a mut Node<'a, T>> {
        self.nodes.get_mut(idx)
    }
}

pub fn poc() {
    println!("{STEP_DESCRIPTION}");

    let mut g: Graph<'_, i32> = Graph::new();
    g.create_node(42);
    g.create_node(43);
    g.create_node(44);

    let n1 = g.get_node(0).unwrap();
    let n2 = g.get_node(1).unwrap();
    println!("{:?}", n1);
    println!("{:?}", n2);

    // Si on décommente la ligne suivante, ça ne compile plus :
    // let n3 = g.get_mut_node(2).unwrap();
    // n3.add_neighbour(n2);
}

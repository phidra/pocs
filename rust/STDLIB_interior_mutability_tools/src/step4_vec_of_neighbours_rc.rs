use std::rc::Rc;

const STEP_DESCRIPTION: &str = r#"
Cette step4 remplace les références par des Rc pour stocker les neighbours.

La [doc](https://doc.rust-lang.org/std/rc/index.html) résume le fonctionnement :
- The type Rc<T> provides shared ownership of a value of type T, allocated in the heap.
- Invoking clone on Rc produces a new pointer to the same allocation in the heap.
- When the last Rc pointer to a given allocation is destroyed, the value stored in that allocation (often referred to as “inner value”) is also dropped.
- Shared references in Rust disallow mutation by default, and Rc is no exception: you cannot generally obtain a mutable reference to something inside an Rc.
- If you need mutability, put a Cell or RefCell inside the Rc

^ En résumé, l'inner value est heap-allocated, immutable, et son ownership est partagé entre tous les Rc clonés.

Ça règle une partie des soucis (on peut facilement accéder aux nodes du graphe), mais on n'y est pas tout à fait.
En effet, comme les Rc sont immutables, on ne peut pas muter les nodes pour leur ajouter des neighbours.
"#;

// un node est paramétré sur une valeur T, et contient une liste de neighbours :
#[derive(Debug)]
#[allow(dead_code)]
struct Node<T> {
    value: T,
    neighbours: Vec<Rc<Node<T>>>,
}

impl<T> Node<T> {
    /// creation with no neighbours :
    fn new(value: T) -> Self {
        Node {
            value,
            neighbours: vec![],
        }
    }

    #[allow(dead_code)]
    fn add_neighbour(&mut self, node: Rc<Node<T>>) {
        self.neighbours.push(node.clone());
    }
}

#[derive(Debug)]
struct Graph<T> {
    nodes: Vec<Rc<Node<T>>>,
}

impl<T> Graph<T> {
    fn new() -> Self {
        Graph { nodes: vec![] }
    }

    fn get_node(&self, idx: usize) -> Option<Rc<Node<T>>> {
        Some(self.nodes.get(idx)?.clone())
    }
}

pub fn poc() {
    println!("{STEP_DESCRIPTION}");

    let mut g: Graph<i32> = Graph::new();
    let n1 = Rc::new(Node::new(42));
    let n2 = Rc::new(Node::new(43));
    let n3 = Rc::new(Node::new(44));
    g.nodes.push(n1);
    g.nodes.push(n2);
    g.nodes.push(n3);

    let n1 = g.get_node(0).unwrap();
    let n2 = g.get_node(1).unwrap();
    println!("{:?}", n1);
    println!("{:?}", n2);


    // Si on décommente ce qui suit, ça ne compile plus :
    // let n3 = g.get_node(2).unwrap();
    // n3.add_neighbour(n2);

}

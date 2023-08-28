use std::rc::Rc;
use std::cell::RefCell;

const STEP_DESCRIPTION: &str = r#"
Avec cette step5, on atteint l'état souhaité = les nodes sont modifiables pour ajouter des neighbours.

Pour cela, on stocke les nodes via Rc<RefCell<Node<T>>>
"#;

// un node est paramétré sur une valeur T, et contient une liste de neighbours :
#[derive(Debug)]
#[allow(dead_code)]
struct Node<T> {
    value: T,
    neighbours: Vec<Rc<RefCell<Node<T>>>>,
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
    fn add_neighbour(&mut self, node: Rc<RefCell<Node<T>>>) {
        self.neighbours.push(node.clone());
    }
}

#[derive(Debug)]
struct Graph<T> {
    nodes: Vec<Rc<RefCell<Node<T>>>>,
}

impl<T> Graph<T> {
    fn new() -> Self {
        Graph { nodes: vec![] }
    }

    fn get_node(&self, idx: usize) -> Option<Rc<RefCell<Node<T>>>> {
        Some(self.nodes.get(idx)?.clone())
    }
}

pub fn poc() {
    println!("{STEP_DESCRIPTION}");

    let mut g: Graph<i32> = Graph::new();
    let n1 = Rc::new(RefCell::new(Node::new(42)));
    let n2 = Rc::new(RefCell::new(Node::new(43)));
    let n3 = Rc::new(RefCell::new(Node::new(44)));
    g.nodes.push(n1);
    g.nodes.push(n2);
    g.nodes.push(n3);

    let n1 = g.get_node(0).unwrap();
    let n2 = g.get_node(1).unwrap();
    let n3 = g.get_node(2).unwrap();


    // on peut enfin définir les neighbours !
    n2.borrow_mut().add_neighbour(n3.clone());
    println!("{:?}", n1);
    println!("{:?}", n2);
    println!("{:?}", n3);

}

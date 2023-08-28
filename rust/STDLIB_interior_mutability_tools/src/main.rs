const POC_DESCRIPTION: &str = r#"

L'objectif de cette POC est de pouvoir définir une structure de type graph en rust.

Au fur et à mesure des étapes, on en arrive à utiliser des Rc<RefCell> pour représenter les neighbours d'un node.

Cette POC est inspirée par [cet article](https://ricardomartins.cc/2016/06/08/interior-mutability).

"#;

mod step1_vec_of_neighbours_values;
use step1_vec_of_neighbours_values as step1;

mod step2_vec_of_neighbours_refs;
use step2_vec_of_neighbours_refs as step2;

mod step3_nodes_are_owned_by_graph;
use step3_nodes_are_owned_by_graph as step3;

mod step4_vec_of_neighbours_rc;
use step4_vec_of_neighbours_rc as step4;

fn main() {
    println!("{POC_DESCRIPTION}");

    println!("\n======== STEP 1 ========");
    step1::poc();

    println!("\n======== STEP 2 ========");
    step2::poc();

    println!("\n======== STEP 3 ========");
    step3::poc();

    println!("\n======== STEP 4 ========");
    step4::poc();
}

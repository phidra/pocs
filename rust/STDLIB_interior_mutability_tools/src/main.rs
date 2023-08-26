const POC_DESCRIPTION: &str = r#"

L'objectif de cette POC est de pouvoir définir une structure de type graph en rust.

On essaye de représenter une adjacency-list.

Inspirée par [cet article](https://ricardomartins.cc/2016/06/08/interior-mutability), .

YYY
ZZZ

"#;

mod step1_vec_of_neighbours_values_fails;
use step1_vec_of_neighbours_values_fails as step1;

mod step2_vec_of_neighbours_refs_fails;
use step2_vec_of_neighbours_refs_fails as step2;


fn main() {
    println!("{POC_DESCRIPTION}");
    step1::poc();
    step2::poc();
}

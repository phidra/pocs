const POC_DESCRIPTION: &str = r#"

POC investiguant ce que le borrow-checker nous laisse faire sur les fields d'une structe parente.

À ma suprise, borrow un field d'une struct ne borrow pas automatiquement le parent.

Dit autrement : si la structe parente n'est elle-même pas borrowed, on peut à la fois :
- borrow un premier field f1 mutably
- borrow un second field f2 mutably
- borrow un troisième field f3 mutably, etc.

Les fields sont donc tous traités indépendamment les uns des autres.

Note qu'indépendamment des borrows concurrentiels, pour pouvoir borrow un field mutably, la struct doit être mutable.

Plus en détail, ce que j'apprends :

- Si la structe parente n'est pas borrowed :
    - le borrow-checker nous laisse manipuler les fields indépendamment les uns des autres
    - on peut donc À LA FOIS borrow un field immutably, et un AUTRE field mutably
    - on peut même borrow plusieurs fields mutably en même temps
    - tout se passe comme si les fields étaient traités indépendamments
- Si la struct parente est borrowed immutably :
    - alors le borrow-checker n'autorise pas à prendre un borrow mutable sur un field
    - (mais on peut prendre des borrow immutables, ce qui est logique)
- Si la struct parente est borrowed mutably :
    - alors le borrow-checker n'autorise pas à prendre un borrow sur un field peu importe qu'il soit mutable ou immutable

"#;


#[derive(Debug)]
struct S {
    strings: Vec<String>,
    ints: Vec<i32>,
}

fn main() {
    println!("{POC_DESCRIPTION}");

    let strings = vec![String::from("Luke"), String::from("Leia")];
    let ints = vec![42, 43];

    // note : la structe parente doit être mut pour pouvoir être borrowed immutably
    let mut mystruct = S{strings, ints};
    println!("INITIAL STRUCT = {:?}", mystruct);

    // dans le scope ci-dessous, la structe parente elle-même n'est pas borrowed :
    {
        let ref1 = &mystruct.strings;
        let ref2: &mut Vec<i32> = &mut mystruct.ints;

        // reading immutable borrow :
        println!("Len of strings = {}", ref1.len());

        // mutating mutable borrow :
        ref2.push(100);

        // reading again immutable borrow :
        println!("Len of strings = {}", ref1.len());

        // mutating again mutable borrow :
        ref2.push(200);

        println!("STRUCT AFTER SCOPE 1 = {:?}", mystruct);
    }

    // dans le scope ci-dessous, la structe parente elle-même est borrowed immutably :
    // (conséquene = les borrow mutables des fields sont interdits, mais les immutables autorisés)
    {
        let parent = &mystruct;
        let ref1 = &mystruct.strings;

        // DÉCOMMENTER CECI POUR VOIR QUE ÇA NE COMPILE PAS :
        // let ref2: &mut Vec<i32> = &mut mystruct.ints;

        // reading immutable borrow :
        println!("Len of strings = {}", ref1.len());

        // DÉCOMMENTER CECI POUR VOIR QUE ÇA NE COMPILE PAS :
        // mutating mutable borrow :
        // ref2.push(100);

        // reading again immutable borrow :
        println!("Len of strings = {}", ref1.len());

        // DÉCOMMENTER CECI POUR VOIR QUE ÇA NE COMPILE PAS :
        // mutating again mutable borrow :
        // ref2.push(200);

        println!("STRUCT AFTER SCOPE 2 = {:?}", parent);
    }

    // dans le scope ci-dessous, la structe parente elle-même est borrowed mutably :
    // (conséquene = aucun borrow de fields n'est autorisé, même immutables)
    {
        let parent = &mut mystruct;

        // DÉCOMMENTER CECI POUR VOIR QUE ÇA NE COMPILE PAS :
        // let ref1 = &mystruct.strings;
        // println!("Len of strings = {}", ref1.len());

        println!("STRUCT AFTER SCOPE 3 = {:?}", parent);
    }

    // juste pour confirmer ma compréhension, dans le scope ci-dessous, la struct parente n'est pas
    // borrowed -> on peut avoir des borrows immutables sur plusieurs fields à la fois :
    {
        let ref1 = &mut mystruct.strings;
        let ref2: &mut Vec<i32> = &mut mystruct.ints;
        ref1.push(String::from("pouet"));
        ref2.push(999);
        ref1.push(String::from("pouet2, le retour"));
        ref2.push(888);

        println!("STRUCT AFTER SCOPE 4 = {:?}", mystruct);
    }
}

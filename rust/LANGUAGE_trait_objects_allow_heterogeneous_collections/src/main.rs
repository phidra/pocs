const POC_DESCRIPTION: &str = r#"

L'objectif de cette POC est d'illustrer que les trait objects permettent de faire une collection d'objets hétérogènes
là où les trait bounds ne le permettent pas.

Ici, TraitBoundPersons est certes générique, mais une même instance de TraitBoundPersons ne contiendra que des personnes du même type.

A contrario, TraitObjectPersons n'est PAS générique, mais contient des pointeurs polymorphiques vers des trucs implémentant Scream.
Ces trucs peuvent être hétérogènes, comme le montre la POC.

"#;

trait Scream {
    fn scream(&self) -> ();
}

struct LoudPerson;
impl Scream for LoudPerson {
    fn scream(&self) -> () {
        println!("HAAAAAAAAAAA !");
    }
}

struct QuietPerson;
impl Scream for QuietPerson {
    fn scream(&self) -> () {
        println!("haaa !");
    }
}

struct TraitBoundPersons<T: Scream> {
    pub persons: Vec<T>,
}

impl<T: Scream> TraitBoundPersons<T> {
    fn scream_all(&self) -> () {
        for person in &self.persons {
            person.scream();
        }
    }
}

fn main() {
    println!("{POC_DESCRIPTION}");

    println!("\nHOMOGENEOUS :");

    // on peut avoir une collection de LoudPerson :
    let mut loud_people: TraitBoundPersons<LoudPerson> = TraitBoundPersons{persons: vec![]};
    loud_people.persons.push(LoudPerson);
    loud_people.persons.push(LoudPerson);
    loud_people.scream_all();

    // on peut avoir une collection de QuietPerson :
    let mut quiet_people: TraitBoundPersons<QuietPerson> = TraitBoundPersons{persons: vec![]};
    quiet_people.persons.push(QuietPerson);
    quiet_people.persons.push(QuietPerson);
    quiet_people.scream_all();


    // ceci ne compilera pas, car loud_people.persons peut contenir soit que des LoudPerson,
    // soit que des QuietPerson, mais pas les deux (et idem pour quiet_people) :
    // loud_people.persons.push(QuietPerson);
    // quiet_people.persons.push(LoudPerson);

    // avec les traits objects, on peut avoir une collection hétérogène :
    struct TraitObjectPerson {
        // ^ la structure n'est PAS générique !
        pub persons: Vec<Box<dyn Scream> >,
    }
    impl TraitObjectPerson {
        fn scream_all(&self) -> () {
            for person in &self.persons {
                person.scream();
            }
        }
    }

    // avec une structure qui stocke des Box<dyn Scream> (qui sont des trait objects), on peut avoir une collection hétérogène :
    println!("\nHETEROGENEOUS :");
    let quiet1 = Box::new(QuietPerson);
    let loud1 = Box::new(LoudPerson);
    let quiet2 = Box::new(QuietPerson);
    let loud2 = Box::new(LoudPerson);
    let heterogeneous_people = TraitObjectPerson{ persons: vec![quiet1, loud1, quiet2, loud2] };
    heterogeneous_people.scream_all();
}

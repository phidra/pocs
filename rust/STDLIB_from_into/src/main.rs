const POC_DESCRIPTION: &str = "

POC sur l'implémentation des traits From et Into (en réalité, From est suffisant).

https://practice.rs/type-conversions/from-into.html

> The From trait allows for a type to define how to create itself from another type

^ permet de convertir un type en un autre.

```
impl From<T> for U
```

^ Cette syntaxe veut dire 'U sait se construire à partir de T'.


> if you have implemented the From trait for your type, then the Into trait will be automatically implemented for the same type.

^ si U sait se construire à partir d'un T, alors T pourra être converti en U. Par exemple :

```
let string1 = String::from(something);
let string2: String = something.into();
```

";


struct Person {
    first_name: String,
    last_name: String,
}

struct Customer {
    company: String,
    first_name: String,
    last_name: String,
}

impl From<Customer> for Person {
    fn from(customer: Customer) -> Person {
        Person{first_name: customer.first_name, last_name: customer.last_name}
    }
}


fn main() {
    println!("{}", POC_DESCRIPTION);
    let our_best_customer = Customer{first_name: String::from("Luke"), last_name: String::from("SKYWALKER"), company: String::from("DeathStar")};
    println!("BEFORE conversion : {} {} from company {}", our_best_customer.first_name, our_best_customer.last_name, our_best_customer.company);
    let person = Person::from(our_best_customer);
    println!("AFTER conversion : M. {} {}", person.first_name, person.last_name);

    // la conversion dans l'autre sens n'est pas possible car le trait n'est implémenté que dans un sens :
    // let customer: Customer = Customer::from(person);
    // println!("Dans l'autre sens : {} {} from company {}", customer.first_name, customer.last_name, customer.company);

    // voici une implémentation du trait qui permettrait la conversion dans l'autre sens :
    // impl From<Person> for Customer {
    //     fn from(person: Person) -> Customer {
    //         Customer{first_name: person.first_name, last_name: person.last_name, company: String::from("unknown")}
    //     }
    // }
}

use std::io;
use rand::Rng;
use std::cmp::Ordering;

fn main() {
    println!("Guess the secret number");
    let mut generator = rand::thread_rng();
    let secret_number = generator.gen_range(1..=100);
    println!("The secret number is: {secret_number}");


    loop {
        println!("Please enter an integer:");
        let mut guessed = String::new();
        io::stdin().read_line(&mut guessed).expect("Error reading stdin");
        let guessed: i32 = match guessed.trim().parse() {
            Ok(value) => value,
            Err(_) => {
                println!("This seems to be an invalid input...");
                continue;
            },
        };

        println!("You guessed {guessed}");

        match guessed.cmp(&secret_number) {
            Ordering::Less => println!("guessed is lesser than secret"),
            Ordering::Greater => println!("guessed is greater than secret"),
            Ordering::Equal => {
                println!("CONGRATULATIONS ! You found the secret number");
                break;
            },
        }
    }
}

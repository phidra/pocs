use std::env;
use std::process;

fn fibo_if(n: i32) -> i32 {
    if n == 0 {
        0
    } else if n == 1 {
        1
    }
    else {
        fibo_if(n-1) + fibo_if(n-2)
    }
}

fn fibo_match(n: i32) -> i32 {
    match n {
        0 => 0,
        1 => 1,
        _ => fibo_match(n-1) + fibo_match(n-2),
    }
}

fn main() {

    let args: Vec<String> = env::args().collect();

    let first_arg = args.get(1);  // Option
    const DEFAULT_N : i32 = 15;
    let n = match first_arg {
        None => { println!("INFO : no argument provided, using default = {}", DEFAULT_N); DEFAULT_N},
        Some(s) => match s.parse::<i32>() {  // Result
            Ok(x) => x,
            Err(_) => { println!("ERROR : arg '{}' is not parsable to integer !", s); process::exit(1); },
        },
    };

    println!("Computing fibo for n = {}", n);
    println!("fibo_if({})    = {}", n, fibo_if(n));
    println!("fibo_match({}) = {}", n, fibo_match(n));
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works_10() {
        let result = fibo_if(10);
        assert_eq!(result, 55);
    }

    #[test]
    fn it_works_15() {
        let result = fibo_if(15);
        assert_eq!(result, 610);
    }

    // this test will fail, this is desired for the POC, to illustrate :
    #[test]
    fn it_does_not_work_15() {
        let result = fibo_if(15);
        assert_eq!(result, 42);
    }
}

#![allow(non_snake_case)]

mod luhn;
mod evaluation;

fn main() {
    println!("{}", luhn::is_valid("12345"));

    let e = evaluation::Expression::Value(32);
    // let to_display = match evaluation::eval(e) {
    //     evaluation::Res::Ok(x) => x,
    //     evaluation::Res::Err(x) => x,
    // };
    println!("{:?}", evaluation::eval(e));
}


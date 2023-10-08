
pub fn is_valid(cc_number: &str) -> bool {
    println!("================== INPUT = {}", cc_number);

    // Ignore all spaces. Reject number with less than two digits.
    let without_spaces = cc_number.split_whitespace().collect::<String>();
    if without_spaces.len() < 2 {
        return false;
    }

    // Moving from right to left, double every second digit: for the number 1234, we double 3 and 1. For the number 98765, we double 6 and 8.
    let mut total_sum = 0;
    let mut should_double = false;
    for c in without_spaces.chars().rev() {
        let digit = match c.to_digit(10) {
            None => return false,
            Some(x) => x,
        };

        if !should_double {
            total_sum += digit;
            should_double = !should_double;
            continue;
        }

        let doubled = digit * 2;

        // After doubling a digit, sum the digits if the result is greater than 9. So doubling 7 becomes 14 which becomes 1 + 4 = 5.
        let summed = if doubled > 9 {
            1 + (doubled - 10)
        } else { doubled };

        // Sum all the undoubled and doubled digits.
        total_sum += summed;
        should_double = !should_double;
    }

    // The credit card number is valid if the sum ends with 0.
    println!("total_sum = {}", total_sum);
    let is_divisible_by_10 = total_sum >= 10 && total_sum % 10 == 0;
    println!("is_divisible_by_10 = {}", is_divisible_by_10);
    let is_valid = total_sum == 0 || is_divisible_by_10;
    println!("is_valid = {}", is_valid);
    is_valid
}

#[test]
fn test_non_digit_cc_number() {
    assert!(!is_valid("foo"));
    assert!(!is_valid("foo 0 0"));
}

#[test]
fn test_empty_cc_number() {
    assert!(!is_valid(""));
    assert!(!is_valid(" "));
    assert!(!is_valid("  "));
    assert!(!is_valid("    "));
}

#[test]
fn test_single_digit_cc_number() {
    assert!(!is_valid("0"));
}

#[test]
fn test_two_digit_cc_number() {
    assert!(is_valid(" 0 0 "));
}

#[test]
fn test_valid_cc_number() {
    assert!(is_valid("4263 9826 4026 9299"));
    assert!(is_valid("4539 3195 0343 6467"));
    assert!(is_valid("7992 7398 713"));
}

#[test]
fn test_invalid_cc_number() {
    assert!(!is_valid("4223 9826 4026 9299"));
    assert!(!is_valid("4539 3195 0343 6476"));
    assert!(!is_valid("8273 1232 7352 0569"));
}

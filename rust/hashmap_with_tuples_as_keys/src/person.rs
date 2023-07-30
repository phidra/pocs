#[derive(Debug, Clone)]
pub struct Person {
    pub first_name: String,
    pub last_name: String,
    pub age: u32,
}

impl Person {
    pub fn display(self: &Self) {
        println!("DISPLAYING : {} {} {}", self.first_name, self.last_name, self.age);
    }
}


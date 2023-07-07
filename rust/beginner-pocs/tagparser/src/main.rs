use std::env;
use std::fs;

fn get_lines(filename: &str) -> Vec<String> {
    let mut results = Vec::new();

    // cf. https://doc.rust-lang.org/rust-by-example/std_misc/file/read_lines.html
    let content = fs::read_to_string(filename);
    for line in content.expect("Unable to read file").lines() {
        results.push(String::from(line));
    }
    results
}

fn is_tag_line(line: &str) -> bool {
    return line.starts_with("- **tags** = ");
}

// precondition = the line is a tag-line :
fn extract_tags(line: String) -> Vec<(String, String)> {
    // 1. remove prefix
    // 2. split with ';'
    // 3. trim leading + trailing whitespaces
    // 4. splitter on the first '>'

    let mut results = Vec::new();
    let tags_part_of_the_line = line.strip_prefix("- **tags** = ").expect("the line should be a tag line");
    let tags_tokens = tags_part_of_the_line.split(";");
    for token in tags_tokens {
        let trimmed = token.trim();
        let splitted_again: Vec<&str> = trimmed.splitn(2, ">").collect();
        if splitted_again.len() < 2 {
            println!("ParsingError : malformed tag = {}", trimmed);
            continue;
        }
        results.push((String::from(splitted_again[0]), String::from(splitted_again[1])));
    }
    results
}

fn main() {
    println!("POC simpliste pour jouer avec le langage");
    println!("");
    println!("OBJECTIF = parser les tags d'un fichier passé en argument");
    println!("");
    println!("CAS NOMINAL :");
    println!("    - charger les lignes du fichier passé en argument");
    println!("    - ignorer toute ligne ne commençant pas par le pattern '- **tags** = '");
    println!("    - pour toute ligne commençant par le pattern, extraire des couples clé + valeur des tags :");
    println!("          - **tags** = language>rust ; topic>language ; level>beginner");
    println!("    - afficher chaque couple clé/valeur");
    println!("");
    println!("CAS D'ERREURS :");
    println!("    - fichier illisible");
    println!("    - tag mal-formé");
    println!("");


    let args: Vec<String> = env::args().collect();

    // file to parse is mandatory :
    let file_to_parse = args.get(1).expect("Missing argument = file path");
    println!("Listing tag lines in file : '{}'", file_to_parse);


    let lines = get_lines(file_to_parse);
    for line in lines {
        println!("");
        if is_tag_line(&line) {
            println!("TAG LINE = {}", line);
            let tags = extract_tags(String::from(line));
            for tag in tags {
                println!("    TAG : {}={}", tag.0, tag.1);
            }
        } else {
            println!("IGNORING NON-TAG LINE = {}", line);
        }
    }
}

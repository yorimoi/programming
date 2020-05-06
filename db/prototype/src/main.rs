mod token;
mod lexer;

fn main() {
    let create_table = r#"CREATE TABLE users (id INT, name TEXT);"#;
    let insert_into  = r#"INSERT INTO users VALUES (1, 'Alice');"#;
    let select       = r#"SELECT id, name FROM users;"#;

    let lexer = lexer::Lexer::new(&create_table);
}

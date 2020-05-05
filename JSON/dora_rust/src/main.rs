mod token;
mod lexer;
mod parser;
mod ast;

fn main() {
    let input = r#"{
        "item1": ["aryitem1", "aryitem2", {"some": {"thing": "coolObj"}}],
        "item2": "simplestringvalue"
    }"#;

    let l = lexer::Lexer::new(&input);
    let mut p = parser::Parser::new(l);

    let result = p.parse_program();
    if let Err(e) = result {
        eprintln!("{}", e);
        std::process::exit(1);
    }

    println!("{:?}", result);
}

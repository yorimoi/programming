mod token;
mod lexer;
mod parser;
mod ast;

fn main() {
    let input = r#"{
        "item1": ["aryitem1", "aryitem2", {"some": {"thing": "coolObj"}}],
        "item2": "simplestringvalue"
    }"#;
    let mut l = lexer::Lexer::new(&input);

    while let t = l.next_token() {
        if t.token_type == token::EOF {
            break;
        }
        print!("{}", t.literal);
    }
}

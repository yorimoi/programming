use std::io::*;
use super::lexer;
use super::ast;
use super::parser;

pub fn run() {
    loop {
        let mut input = String::new();
        print!("> ");
        stdout().flush().unwrap();
        stdin().read_line(&mut input).ok();

        match &*input {
            "exit\n" => return,
            "\n" => continue,
            _ => (),
        }

        let mut l = lexer::Lexer::new(&input);
        let mut tok = lexer::tokenize(&mut l);
        //println!("{:?}", tok);

        let ast = ast::eval(&mut tok);
        let mut vec: Vec<i64> = Vec::new();
        parser::gen(&ast, &mut vec);

        let output = vec.pop().unwrap();
        println!("{}", output);
    }
}

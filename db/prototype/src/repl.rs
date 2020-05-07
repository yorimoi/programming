
use crate::token;
use crate::lexer;
use crate::parser;

use std::io::{stdout, Write};

pub fn run() {
    let prompt = ">> ";

    loop {
        print!("{}", prompt);
        stdout().flush().unwrap();

        let input = {
            let mut s = String::new();
            std::io::stdin().read_line(&mut s).ok();
            s
        };

        let mut l = lexer::Lexer::new(&input);
        loop {
            let token = l.next_token();
            if token.kind == token::TokenKind::EOF {
                break;
            }
            print!("{:?}", token);
        }
        println!();
    }
}

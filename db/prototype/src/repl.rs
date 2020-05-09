
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

        let ast = parser::parse(&input);
        println!("{:?}", ast);
    }
}

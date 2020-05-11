
use crate::ast::AstKind;
use crate::parser;
use crate::table;
use crate::display;

use std::io::{stdout, Write};

pub fn run() {
    let prompt = ">> ";
    let mut table: table::Table = Default::default();

    println!("Welcome to prototypesql");
    println!("Use `exit` to exit\n");

    loop {
        print!("{}", prompt);
        stdout().flush().unwrap();

        let input = {
            let mut s = String::new();
            std::io::stdin().read_line(&mut s).ok();
            s
        };

        match &*input {
            "\n" => continue,
            "exit\n" => break,
            _ => (),
        }

        let ast = match parser::parse(&input) {
            Ok(ast) => ast,
            Err(e) => {
                eprintln!("{}", e);
                continue;
            },
        };

        for stmt in ast.statements {
            match stmt.kind {
                AstKind::Select => {
                    match table.select(&stmt.select.unwrap()) {
                        Ok(virtual_table) => {
                            display::view(&virtual_table);
                        },
                        Err(e) => eprintln!("{}", e),
                    }
                },
                AstKind::Insert => {
                    match table.insert(&stmt.insert.unwrap()) {
                        Ok(_) => println!("ok"),
                        Err(e) => eprintln!("{}", e),
                    }
                },
                AstKind::Create => {
                    match table::Table::create_table(&stmt.create.unwrap()) {
                        Ok(t) => {
                            println!("ok");
                            table = t;
                        },
                        Err(e) => eprintln!("{}", e),
                    };
                },
            }
        }
    }
}

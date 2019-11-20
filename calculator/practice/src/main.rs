//extern crate practice;

mod lexer;
mod token;
mod ast;
mod parser;
mod repl;

fn main() {
    let args: Vec<String> = std::env::args().collect();

    if args.len() == 1 {
        println!("Caluculator practice REPL");
        println!("Type \"exit\" to exit");
        repl::run();
    } else {
        use std::fs::File;
        use std::io::prelude::*;
        let mut f = File::open(&args[1]).unwrap();
        let mut input = String::new();
        f.read_to_string(&mut input).unwrap();
        let input: Vec<&str> = input.split('\n').collect();

        for line in &input {
            let mut l = lexer::Lexer::new(&line);
            let mut tok = lexer::tokenize(&mut l);
            //println!("{:?}", tok);
            if tok.len() == 1 {
                continue;
            }
            println!("> {}", line);

            let ast = ast::eval(&mut tok);
            let mut vec: Vec<f64> = Vec::new();
            parser::gen(&ast, &mut vec);

            let output = vec.pop().unwrap();
            println!("{}", output);
        }
    }
}

/// cargo test -- --nocapture
#[test]
fn test_main() {
    let mut ng_count = 0;

    ng_count += assert(0,  "0");
    ng_count += assert(42, "42");
    ng_count += assert(21, "5+20-4");
    ng_count += assert(41, "12 + 34 - 5");
    ng_count += assert(47, "5+6*7");
    ng_count += assert(15, "5*(9-6)");
    ng_count += assert(4,  "(3+5)/2");
    ng_count += assert(-3, "5-(1-3)-10");
    ng_count += assert(3,  "-1-(-4)");
    ng_count += assert(7,  "4.2+2.8");
    ng_count += assert(10, "0.001+9.999");

    if ng_count == 0 {
        println!("\x1b[36mSUCCESS\x1b[0m");
    } else {
        println!("\x1b[31mFAIL\x1b[0m ... {}", ng_count);
        panic!("");
    }
}

#[cfg(test)]
fn assert(expected: i32, args: &str) -> i32 {
    let mut l = lexer::Lexer::new(&args);
    let mut tok = lexer::tokenize(&mut l);
    //println!("{:?}", tok);

    let ast = ast::eval(&mut tok);

    let mut vec: Vec<f64> = Vec::new();
    parser::gen(&ast, &mut vec);

    let actual = vec.pop().unwrap() as i32;
    let is_ng = if actual == expected {
        print!("[  \x1b[32mOK\x1b[0m  ] ");
        println!("{} => {}", args, actual);
        0
    } else {
        print!("[  \x1b[31mNG\x1b[0m  ] ");
        println!("{} => expected {}, but got {}", args, expected,  actual);
        1
    };

    is_ng
}

use crate::ast::AstKind;
use crate::parser;
use crate::table;
use crate::display;

use std::net::{TcpListener, TcpStream, Shutdown};
use std::io::prelude::*;

pub fn run() {
    let listener = TcpListener::bind("localhost:5432").unwrap();
    let mut table: table::Table = Default::default();

    println!("Server listening on port 5432");

    for stream in listener.incoming() {
        let stream = stream.unwrap();

        handle_connection(stream, &mut table);
    }

    drop(listener);
}

fn handle_connection(mut stream: TcpStream, table: &mut table::Table) {
    let mut data = [0; 256];

    while match stream.read(&mut data) {
        Ok(size) => {
            if size < 2 {
                return;
            }

            let input = String::from_utf8_lossy(&data[0..size]);

            let ast = match parser::parse(&input) {
                Ok(ast) => ast,
                Err(e) => {
                    eprintln!("{}", e);
                    return;
                },
            };

            let mut ret = String::new();

            for stmt in ast.statements {
                match stmt.kind {
                    AstKind::Select => {
                        match table.select(&stmt.select.unwrap()) {
                            Ok(virtual_table) => {
                                ret += &display::html(&virtual_table);
                            },
                            Err(e) => ret += &format!("{}", e),
                        }
                    },
                    AstKind::Insert => {
                        match table.insert(&stmt.insert.unwrap()) {
                            Ok(_) => ret += "ok<br />",
                            Err(e) => ret += &format!("{}", e),
                        }
                    },
                    AstKind::Create => {
                        match table::Table::create_table(&stmt.create.unwrap()) {
                            Ok(t) => {
                                ret += "ok<br />";
                                *table = t;
                            },
                            Err(e) => ret += &format!("{}", e),
                        };
                    },
                }
            }

            stream.write(ret.as_bytes()).unwrap();

            true
        },
        Err(_) => {
            eprintln!("Server error occurred, terminating connection with {}", stream.peer_addr().unwrap());
            stream.shutdown(Shutdown::Both).unwrap();
            false
        },
    } {}
}

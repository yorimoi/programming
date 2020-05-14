extern crate prototype;

use std::env;
use prototype::repl;
use prototype::server;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() == 1 {
        repl::run();
    } else if args[1] == "server" {
        server::run();
    } else {
        eprintln!("Invalid argument");
        std::process::exit(1);
    }
}

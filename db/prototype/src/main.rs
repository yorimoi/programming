extern crate prototype;
extern crate clap;

use clap::{App, Arg};

use prototype::repl;
use prototype::server;

fn main() {
    let app = App::new("prototype")
        .arg(Arg::with_name("server")
            .help("Start server")
            .short("s")
            .long("server")
        );

    let matches = app.get_matches();

    if matches.is_present("server") {
        server::run();
    } else {
        repl::run();
    }
}

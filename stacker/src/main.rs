use std::{thread, time};
use std::sync::{Arc, Mutex};

use getch::Getch;

const FIELD_WIDTH:  usize = 20;
const FIELD_HEIGHT: usize = 20;

enum Dir {
    Right,
    Left,
}

struct Field {
    pub field:        [u32; FIELD_HEIGHT],
    pub current_line: usize,
    pub line_length:  usize,
    pub line_begin:   usize,
    pub line_end:     usize,
    pub dir:          Dir,
    pub interval:     u64,
}

impl Field {
    fn new() -> Self {
        let mut field = [0; FIELD_HEIGHT];
        field[0] = std::u32::MAX;

        Self {
            field,
            current_line: 1,
            line_length : 8,  // ########
                              // ^^^^^^^^ 8
            line_begin:   1,  // ########
                              // ^ 1    ^ 8
            line_end:     8,
            dir:          Dir::Right,
            interval:     100,
        }
    }
}

fn display(f: &Field) {
    println!("\x1b[H");
    println!("\x1b[31m-{:-<width$}-\x1b[0m", "-", width = FIELD_WIDTH);

    for h in (f.current_line..FIELD_HEIGHT-1).rev() {
        print!("|");
        for w in 0..FIELD_WIDTH {
            if h == f.current_line && f.line_begin <= w && w <= f.line_end {
                print!("#");
            } else {
                print!(" ");
            }
        }
        print!("|\n");
    }

    for h in (1..f.current_line).rev() {
        print!("|\x1b[34m");
        for w in 0..FIELD_WIDTH {
            if f.field[h] & (1 << w) != 0 {
                print!("#");
            } else {
                print!(" ");
            }
        }
        print!("\x1b[0m|\n");
    }

    println!("-{:-<width$}-", "-", width = FIELD_WIDTH);
}

fn gameloop() {
    let field = Arc::new(Mutex::new(Field::new()));

    let field_clone = Arc::clone(&field);
    let _ = thread::spawn(move || {
        loop {
            display(&field_clone.lock().unwrap());

            let interval = field_clone.lock().unwrap().interval;
            thread::sleep(time::Duration::from_millis(interval));

            let mut field = field_clone.lock().unwrap();
            match field.dir {
                Dir::Right => {
                    if field.line_end < FIELD_WIDTH-1 {
                        field.line_begin += 1;
                    } else {
                        field.dir = Dir::Left;
                        field.line_begin -= 1;
                    }
                },
                Dir::Left => {
                    if 0 < field.line_begin {
                        field.line_begin -= 1;
                    } else {
                        field.dir = Dir::Right;
                        field.line_begin += 1;
                    }
                },
            }
            field.line_end = field.line_begin + field.line_length;
        }
    });

    let g = Getch::new();
    loop {
        match g.getch() {
            Ok(c) => {
                match c {
                    b'q' => return,
                    b' ' => {
                        let mut field = field.lock().unwrap();
                        let current_line = field.current_line;

                        // Fixed line
                        for i in 0..=field.line_length {
                            field.field[current_line] |= (1 << field.line_begin+i) & field.field[current_line-1];
                        }

                        // Judge
                        if field.field[current_line] == 0 {
                            // continue; // for debug
                            display(&field);
                            println!("     GAME OVER!!");
                            return;
                        }
                        if field.current_line == FIELD_HEIGHT-2 {
                            display(&field);
                            println!("     GAME CLEAR!!");
                            return;
                        }

                        field.current_line += 1;

                        field.interval = (field.interval as f64 * 0.9) as u64;

                        if field.current_line % 3 == 0 {
                            field.line_length -= 1;
                        }
                    },
                    _ => (),  // do nothing
                }
            },
            Err(_) => (), // ignore
        }
    }
}

fn main() {
    if 32 <= FIELD_WIDTH {
        panic!(format!("invalid width: {}", FIELD_WIDTH));
    }

    println!("\x1b[2J\x1b[H\x1b[?25l");

    gameloop();

    println!("\x1b[?25h");
}


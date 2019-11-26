extern crate ncurses;
extern crate rand;

use std::{thread, time};
use std::sync::{Arc, Mutex};
use ncurses::*;
use rand::Rng;

const H: usize = 20 + 1 + 2;  // field + wall + sentinel
const W: usize = 10 + 2 + 2;  // field + wall + sentinel

#[derive(Clone)]
struct Tetris {
    field: [[i32; W]; H],
    shape: [i32; 4],
    x: i32,
    y: i32,
}

fn init_mino(t: &mut Tetris) {
    /*   8421
     * 0 0000
     * 1 0000 */
    static MINOS: [[i32; 2]; 7] = [
        [ 0, 15],  // I
        [ 6,  6],  // O
        [ 6, 12],  // S
        [12,  6],  // Z
        [14,  2],  // J
        [ 2, 14],  // L
        [ 4, 14],  // T
    ];

    let rnd = rand::thread_rng().gen_range(0, MINOS.len());

    t.shape[0] = 0;
    t.shape[1] = MINOS[rnd][0];
    t.shape[2] = MINOS[rnd][1];
    t.shape[3] = 0;

    t.x = 4;
    t.y = 0;
}

#[allow(deprecated)]
fn draw(t: &Tetris) {
    let mut field_buf: [[i32; W]; H] = [[0; W]; H];

    for y in 0..H {
        for x in 0..W {
            field_buf[y][x] = t.field[y][x];
        }
    }

    for y in 0..4 {
        for x in 0..4 {
            field_buf[y+t.y as usize][x+t.x as usize] |= t.shape[y] >> 3-x & 1;
        }
    }

    //print!("\x1b[H");
    clear();
    for y in 1..H-1 {
        for x in 1..W-1 {
            if field_buf[y][x] == 0 {
                printw(" .");
            } else {
                printw("[]");
            }
        }
        printw("\n");
    }
    refresh();
}

fn is_collision(t: &Tetris, tx: i32, ty: i32, s: &[i32]) -> bool {
    let tx = t.x + tx;
    let ty = t.y + ty;
    for y in 0..4 {
        for x in 0..4 {
            if x+tx < 0 || W as i32 <= x+tx || y+ty < 0 || H as i32 <= y+ty {
                continue;
            }
            if t.field[(y+ty) as usize][(x+tx) as usize]
                & (s[y as usize] >> 3-x & 1) == 1 {
                return true;
            }
        }
    }
    false
}

fn gameloop(t: &mut Arc<Mutex<Tetris>>) {

    draw(&t.lock().unwrap());

    // Fall
    {
        let t = Arc::clone(&t);
        let _ = thread::spawn(move || {
            loop {
                thread::sleep(time::Duration::from_millis(500));
                let mut t = t.lock().unwrap();
                if !is_collision(&t, 0, 1, &t.shape) {
                    t.y += 1;
                    draw(&t);
                } else {
                    // Fix to field
                    let ty = t.y as usize;
                    let tx = t.x as usize;
                    for y in 0..4 {
                        for x in 0..4 {
                            t.field[y+ty][x+tx] |= t.shape[y] >> 3-x & 1;
                        }
                    }
                    // Erase line
                    //for y in (1..H-2).rev() {
                    let mut y = H-3;
                    while y > 0 {
                        let mut can_erase = true;
                        for x in 2..W-3 {
                            if t.field[y][x] == 0 {
                                can_erase = false;
                                break;
                            }
                        }
                        if can_erase {
                            for y2 in (1..y+1).rev() {
                                for x2 in 2..W-3 {
                                    t.field[y2][x2] = t.field[y2-1][x2];
                                }
                            }
                            y += 1;
                        }
                        y -= 1;
                    }
                    // Spawn mino
                    init_mino(&mut t);
                }
            }
        });
    }

    // Key operation
    let t = Arc::clone(&t);
    loop {
        let mut tx = 0;
        let mut ty = 0;
        match getch() as u8 {
            b'q' => return,
            b'h' => tx = -1,
            b'j' => ty =  1,
            b'k' => ty = -1,
            b'l' => tx =  1,
            b' ' => {
                // Right rotation
                let mut t = t.lock().unwrap();
                let mut s: [i32; 4] = [0; 4];
                for y in 0..4 {
                    for x in 0..4 {
                        s[4-1-x] |= (t.shape[y]>>x & 1) << y;
                    }
                }
                if !is_collision(&t, 0, 0, &s) {
                    t.shape = s;
                }
                continue;
            },
            _  => (),
        }
        let mut t = t.lock().unwrap();
        if !is_collision(&t, tx, ty, &t.shape) {
            t.x += tx;
            t.y += ty;
            draw(&t);
        }
    }
}

fn main() {

    let mut t = Arc::new(Mutex::new(
        Tetris {field: [[0; W]; H], shape: [0; 4], x: 0, y: 0}));

    init_mino(&mut t.lock().unwrap());

    for y in 0..H {
        t.lock().unwrap().field[y][1]   = 1;
        t.lock().unwrap().field[y][W-2] = 1;
    }
    for x in 0..W {
        t.lock().unwrap().field[H-2][x] = 1;
    }

    //print!("\x1b[2J\x1b[H");
    initscr();
    noecho();

    gameloop(&mut t);

    endwin();
}
